#include "Region/VRegionClient.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "HAL/IConsoleManager.h"
#include "GameFramework/PlayerController.h"
#include "Interface/VSpatialItemActorInterface.h"
#include "Presentation/VMaterialPresenterApplier.h"
#include "Presentation/VMaterialResolver.h"
#include "Region/VRegionClientBridge.h"
#include "Region/VRegionClientResolver.h"
#include "Region/VRegionRenderQueue.h"
#include "Subsystem/VAssetManager.h"
#include "Utility/VSpatialActorEvents.h"

DEFINE_LOG_CATEGORY_STATIC(LogVRegionClient, Log, All);

namespace
{
	TAutoConsoleVariable<int32> CVarRegionClientJobsPerTick(
		TEXT("vw.regionclient.jobs_per_tick"),
		64,
		TEXT("Jobs to process per tick for AVRegionClient."),
		ECVF_Default);

	TAutoConsoleVariable<int32> CVarRegionClientRenderBudget(
		TEXT("vw.regionclient.render_budget"),
		40,
		TEXT("Render budget for AVRegionClient."),
		ECVF_Default);

	TAutoConsoleVariable<int32> CVarRegionClientDebug(
		TEXT("vw.regionclient.debug"),
		0,
		TEXT("Enable debug logging for AVRegionClient."),
		ECVF_Default);

	bool IsRegionClientDebugEnabled()
	{
		return CVarRegionClientDebug.GetValueOnGameThread() != 0;
	}

	void LogRegionClientDebugState(const TCHAR* Context, int32 RenderQueueSize, int32 RenderAppliedCount)
	{
		if (!IsRegionClientDebugEnabled())
		{
			return;
		}

		const int32 JobsPerTick = CVarRegionClientJobsPerTick.GetValueOnGameThread();
		const int32 RenderBudget = CVarRegionClientRenderBudget.GetValueOnGameThread();

		const int32 PendingMeshQueueSize = 0;
		const int32 PendingMaterialQueueSize = 0;
		const int32 DrainedMeshJobsThisTick = 0;
		const int32 DrainedMaterialJobsThisTick = 0;

		UE_LOG(
			LogVRegionClient,
			Log,
			TEXT("AVRegionClient[%s] CVar jobs_per_tick=%d render_budget=%d | Queues(mesh=%d material=%d render=%d) | Drain(mesh=%d material=%d render=%d)"),
			Context,
			JobsPerTick,
			RenderBudget,
			PendingMeshQueueSize,
			PendingMaterialQueueSize,
			RenderQueueSize,
			DrainedMeshJobsThisTick,
			DrainedMaterialJobsThisTick,
			RenderAppliedCount);
	}
} // namespace

AVRegionClient::AVRegionClient()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AVRegionClient::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (!World || World->GetNetMode() != NM_Client)
	{
		return;
	}

	AssetManager = World->GetSubsystem<UVAssetManager>();

	if (APlayerController* PC = World->GetFirstPlayerController())
	{
		RegionBridge = NewObject<URegionClientBridge>(this);
		RegionBridge->Initialize(PC);
	}

	RegionClientResolver = NewObject<UVRegionClientResolver>(this);
	RegionClientResolver->Initialize(AssetManager);

	MaterialResolver = NewObject<UVMaterialResolver>(this);
	MaterialResolver->Initialize(AssetManager, RegionBridge);

	MaterialPresenter = NewObject<UVMaterialPresenterApplier>(this);
	MaterialPresenter->Initialize(MaterialResolver);

	PostInitHandle = FSpatialActorEvents::OnSpatialActorPostInit().AddUObject(
		this, &AVRegionClient::HandleSpatialActorPostInit);
	MeshAssetHandle = FSpatialActorEvents::OnSpatialActorMeshAssetIdChanged().AddUObject(
		this, &AVRegionClient::HandleMeshAssetIdChanged);
	MaterialIdsHandle = FSpatialActorEvents::OnSpatialActorMaterialIdsChanged().AddUObject(
		this, &AVRegionClient::HandleMaterialIdsChanged);

	LogRegionClientDebugState(TEXT("BeginPlay"), RenderQueue.Num(), 0);
}

void AVRegionClient::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (PostInitHandle.IsValid())
	{
		FSpatialActorEvents::OnSpatialActorPostInit().Remove(PostInitHandle);
	}
	if (MeshAssetHandle.IsValid())
	{
		FSpatialActorEvents::OnSpatialActorMeshAssetIdChanged().Remove(MeshAssetHandle);
	}
	if (MaterialIdsHandle.IsValid())
	{
		FSpatialActorEvents::OnSpatialActorMaterialIdsChanged().Remove(MaterialIdsHandle);
	}

	Super::EndPlay(EndPlayReason);
}

void AVRegionClient::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	const int32 JobsPerTick = CVarRegionClientJobsPerTick.GetValueOnGameThread();
	const int32 JobsToDrain = FMath::Max(0, JobsPerTick);
	const int32 QueueSizeBefore = JobQueue.Num();
	const int32 JobsDrainedThisTick = FMath::Min(QueueSizeBefore, JobsToDrain);
	TArray<FVRegionClientJob> DrainedJobs;

	if (JobsDrainedThisTick > 0)
	{
		DrainedJobs.Reserve(JobsDrainedThisTick);
		for (int32 Index = 0; Index < JobsDrainedThisTick; ++Index)
		{
			DrainedJobs.Add(MoveTemp(JobQueue[Index]));
		}

		JobQueue.RemoveAt(0, JobsDrainedThisTick, EAllowShrinking::No);
		CoalescedJobIndices.Reset();
		for (int32 Index = 0; Index < JobQueue.Num(); ++Index)
		{
			if (ShouldCoalesceJob(JobQueue[Index].JobType))
			{
				CoalescedJobIndices.Add({JobQueue[Index].ItemId, JobQueue[Index].JobType}, Index);
			}
		}
	}

	if (RegionClientResolver)
	{
		for (const FVRegionClientJob& Job : DrainedJobs)
		{
			RegionClientResolver->ConsumeJob(Job);
		}
	}

	if (RegionClientResolver)
	{
		RegionClientResolver->EmitRenderWork(RenderQueue, AssetManager);
	}

	const int32 RenderBudget = FMath::Max(0, CVarRegionClientRenderBudget.GetValueOnGameThread());
	int32 RenderAppliedCount = 0;
	int32 RenderBudgetRemaining = RenderBudget;
	RenderQueue.Drain(
		RenderBudget,
		RenderAppliedCount,
		RenderBudgetRemaining,
		[this](const FVRegionRenderWorkItem& Item)
		{
			return ApplyRenderWork(Item);
		});

	if (IsRegionClientDebugEnabled())
	{
		UE_LOG(
			LogVRegionClient,
			Log,
			TEXT("AVRegionClient Tick drain queue_before=%d queue_after=%d drained=%d render_queue=%d render_applied=%d"),
			QueueSizeBefore,
			JobQueue.Num(),
			JobsDrainedThisTick,
			RenderQueue.Num(),
			RenderAppliedCount);
	}
}

void AVRegionClient::OnMaterialsBatchReceived(const TArray<FVMMaterial>& Materials)
{
	FVRegionClientJob Job;
	Job.JobType = EVRegionClientJobType::MaterialsBatchReceived;
	Job.MaterialsBatch = Materials;
	EnqueueJob(MoveTemp(Job));

	if (MaterialResolver)
	{
		MaterialResolver->OnMaterialsBatchReceived(Materials);
	}
}

AVRegionClient* AVRegionClient::Get(const UObject* WorldContext)
{
	if (!WorldContext)
	{
		return nullptr;
	}

	UWorld* World = WorldContext->GetWorld();
	if (!World)
	{
		return nullptr;
	}

	for (TActorIterator<AVRegionClient> It(World); It; ++It)
	{
		return *It;
	}

	return nullptr;
}

void AVRegionClient::HandleSpatialActorPostInit(AActor* Actor, const FSpatialItemId& SpatialId)
{
	if (!Actor || !GetWorld() || GetWorld()->GetNetMode() != NM_Client)
	{
		return;
	}

	const FGuid ItemId = ResolveSpatialId(Actor, &SpatialId);
	if (!ItemId.IsValid())
	{
		return;
	}

	{
		FVRegionClientJob Job;
		Job.JobType = EVRegionClientJobType::SpatialActorObserved;
		Job.ItemId = ItemId;
		Job.Actor = Actor;
		EnqueueJob(MoveTemp(Job));
	}

	RegisterActor(Actor, ItemId);
}

void AVRegionClient::HandleMeshAssetIdChanged(AActor* Actor, const FMeshAssetId& MeshAssetId)
{
	if (!Actor)
	{
		return;
	}

	const FGuid ItemId = ResolveSpatialId(Actor);
	if (!ItemId.IsValid())
	{
		return;
	}

	{
		FVRegionClientJob Job;
		Job.JobType = EVRegionClientJobType::MeshAssetIdChanged;
		Job.ItemId = ItemId;
		Job.MeshAssetId = MeshAssetId;
		Job.Actor = Actor;
		EnqueueJob(MoveTemp(Job));
	}

}

void AVRegionClient::HandleMaterialIdsChanged(AActor* Actor, const TArray<uint32>& MaterialIds)
{
	if (!Actor)
	{
		return;
	}

	const FGuid ItemId = ResolveSpatialId(Actor);
	if (!ItemId.IsValid())
	{
		return;
	}

	{
		FVRegionClientJob Job;
		Job.JobType = EVRegionClientJobType::MaterialIdsChanged;
		Job.ItemId = ItemId;
		Job.MaterialIdsBySlot = MaterialIds;
		Job.Actor = Actor;
		EnqueueJob(MoveTemp(Job));
	}
}

void AVRegionClient::HandleActorDestroyed(AActor* Actor)
{
	if (!Actor)
	{
		return;
	}

	const TWeakObjectPtr<AActor> ActorKey(Actor);
	if (const FGuid* ItemId = ActorToSpatialId.Find(ActorKey))
	{
		FVRegionClientJob Job;
		Job.JobType = EVRegionClientJobType::ActorDestroyed;
		Job.ItemId = *ItemId;
		Job.Actor = Actor;
		EnqueueJob(MoveTemp(Job));

		SpatialItemActors.Remove(*ItemId);
		VisualStates.Remove(*ItemId);
		ActorToSpatialId.Remove(ActorKey);

		if (MaterialPresenter)
		{
			if (ISpatialMeshItemActorInterface* MeshInterface = Cast<ISpatialMeshItemActorInterface>(Actor))
			{
				if (UStaticMeshComponent* MeshComp = MeshInterface->GetSpatialMeshComponent())
				{
					MaterialPresenter->ForgetMeshComponent(MeshComp);
				}
			}
		}
	}
}

void AVRegionClient::EnqueueJob(FVRegionClientJob&& Job)
{
	const EVRegionClientJobType JobType = Job.JobType;
	if (Job.Sequence == 0)
	{
		Job.Sequence = ++JobSequence;
	}

	if (ShouldCoalesceJob(JobType))
	{
		const FVRegionClientJobKey Key{Job.ItemId, JobType};
		if (int32* ExistingIndex = CoalescedJobIndices.Find(Key))
		{
			JobQueue[*ExistingIndex] = MoveTemp(Job);
		}
		else
		{
			const int32 NewIndex = JobQueue.Add(MoveTemp(Job));
			CoalescedJobIndices.Add(Key, NewIndex);
		}
	}
	else
	{
		JobQueue.Add(MoveTemp(Job));
	}

	if (IsRegionClientDebugEnabled())
	{
		const FString ItemIdString = Job.ItemId.IsValid() ? Job.ItemId.ToString() : TEXT("None");
		UE_LOG(
			LogVRegionClient,
			Log,
			TEXT("AVRegionClient enqueued job type=%d item=%s queue_size=%d"),
			static_cast<int32>(JobType),
			*ItemIdString,
			JobQueue.Num());
	}
}

bool AVRegionClient::ShouldCoalesceJob(EVRegionClientJobType JobType) const
{
	return JobType == EVRegionClientJobType::MeshAssetIdChanged
		|| JobType == EVRegionClientJobType::MaterialIdsChanged;
}

void AVRegionClient::RegisterActor(AActor* Actor, const FGuid& ItemId)
{
	if (!Actor)
	{
		return;
	}

	const TWeakObjectPtr<AActor> ActorKey(Actor);
	SpatialItemActors.FindOrAdd(ItemId) = Actor;
	ActorToSpatialId.FindOrAdd(ActorKey) = ItemId;

	Actor->OnDestroyed.AddUniqueDynamic(this, &AVRegionClient::HandleActorDestroyed);
}

FGuid AVRegionClient::ResolveSpatialId(AActor* Actor, const FSpatialItemId* SpatialIdOverride)
{
	if (!Actor)
	{
		return FGuid();
	}

	if (SpatialIdOverride)
	{
		const FGuid OverrideId = SpatialIdOverride->Value.Value;
		if (OverrideId.IsValid())
		{
			return OverrideId;
		}
	}

	const TWeakObjectPtr<AActor> ActorKey(Actor);
	if (const FGuid* ItemId = ActorToSpatialId.Find(ActorKey))
	{
		return *ItemId;
	}

	if (ISpatialItemActorInterface* SpatialInterface = Cast<ISpatialItemActorInterface>(Actor))
	{
		const FGuid InterfaceId = SpatialInterface->GetSpatialItemId().Value.Value;
		if (InterfaceId.IsValid())
		{
			RegisterActor(Actor, InterfaceId);
			return InterfaceId;
		}
	}

	return FGuid();
}

void AVRegionClient::SyncVisualStateFromActor(AActor* Actor, const FGuid& ItemId)
{
	if (!Actor)
	{
		return;
	}

	if (ISpatialMeshItemActorInterface* MeshInterface = Cast<ISpatialMeshItemActorInterface>(Actor))
	{
		FSpatialActorVisualState& State = VisualStates.FindOrAdd(ItemId);
		State.MeshAssetId = MeshInterface->GetSpatialMeshAssetId();
		State.MaterialIdsBySlot = MeshInterface->GetSpatialMaterialIdsBySlot();
		State.PendingMaterialSlots.Reset();
		State.PendingMaterialSlots.Reserve(State.MaterialIdsBySlot.Num());
		for (int32 Index = 0; Index < State.MaterialIdsBySlot.Num(); ++Index)
		{
			State.PendingMaterialSlots.Add(Index);
		}
		State.bMeshPending = true;
		State.bMaterialsPending = true;

		TryApplyMesh(ItemId, Actor, State);
		TryApplyMaterials(ItemId, Actor, State);
	}
}

bool AVRegionClient::TryApplyMesh(const FGuid& ItemId, AActor* Actor, FSpatialActorVisualState& State) const
{
	if (!Actor || !AssetManager)
	{
		return false;
	}

	if (ISpatialMeshItemActorInterface* MeshInterface = Cast<ISpatialMeshItemActorInterface>(Actor))
	{
		const FGuid MeshId = State.MeshAssetId.Value.Value;
		if (!MeshId.IsValid())
		{
			return false;
		}

		if (UStaticMeshComponent* MeshComp = MeshInterface->GetSpatialMeshComponent())
		{
			if (UStaticMesh* MeshAsset = AssetManager->GetCachedStaticMesh(MeshId))
			{
				MeshComp->SetStaticMesh(MeshAsset);
				State.bMeshPending = false;
				return true;
			}
		}
	}

	return false;
}

bool AVRegionClient::TryApplyMaterials(const FGuid& ItemId, AActor* Actor, FSpatialActorVisualState& State) const
{
	if (!Actor || !MaterialPresenter)
	{
		return false;
	}

	if (ISpatialMeshItemActorInterface* MeshInterface = Cast<ISpatialMeshItemActorInterface>(Actor))
	{
		if (UStaticMeshComponent* MeshComp = MeshInterface->GetSpatialMeshComponent())
		{
			if (!State.bForceMaterialReapply && State.PendingMaterialSlots.Num() == 0)
			{
				State.bMaterialsPending = false;
				return false;
			}

			if (State.bForceMaterialReapply)
			{
				TArray<int32> AllSlots;
				AllSlots.Reserve(State.MaterialIdsBySlot.Num());
				for (int32 Index = 0; Index < State.MaterialIdsBySlot.Num(); ++Index)
				{
					AllSlots.Add(Index);
				}
				MaterialPresenter->ApplyMaterialsForSlots(MeshComp, State.MaterialIdsBySlot, AllSlots);
				State.bForceMaterialReapply = false;
				State.PendingMaterialSlots.Reset();
			}
			else if (State.PendingMaterialSlots.Num() > 0)
			{
				MaterialPresenter->ApplyMaterialsForSlots(MeshComp, State.MaterialIdsBySlot, State.PendingMaterialSlots);
				State.PendingMaterialSlots.Reset();
			}
			State.bMaterialsPending = false;
			return true;
		}
	}

	return false;
}

bool AVRegionClient::ApplyRenderWork(const FVRegionRenderWorkItem& Item)
{
	if (!RegionClientResolver)
	{
		return false;
	}

	UVRegionClientResolver::FRegionClientItemSnapshot Snapshot;
	if (!RegionClientResolver->GetItemSnapshot(Item.ItemId, Snapshot))
	{
		return false;
	}

	TWeakObjectPtr<AActor> ActorPtr = Snapshot.Actor;
	if (!ActorPtr.IsValid())
	{
		if (const TWeakObjectPtr<AActor>* FallbackActor = SpatialItemActors.Find(Item.ItemId))
		{
			ActorPtr = *FallbackActor;
		}
	}

	AActor* Actor = ActorPtr.Get();
	bool bApplied = false;

	switch (Item.WorkType)
	{
	case EVRegionRenderWorkType::SetMesh:
		bApplied = ApplyMeshWork(Item, Actor);
		break;
	case EVRegionRenderWorkType::ApplyMaterials:
		bApplied = ApplyMaterialsWork(Item, Actor);
		break;
	default:
		break;
	}

	if (bApplied)
	{
		RegionClientResolver->MarkApplied(Item.ItemId, Item.WorkType);
		return true;
	}

	if (RegionClientResolver->MarkLegacyFallbackAttempted(Item.ItemId, Item.WorkType))
	{
		return ApplyLegacyFallback(Item, Actor, Snapshot);
	}

	return false;
}

bool AVRegionClient::ApplyMeshWork(const FVRegionRenderWorkItem& Item, AActor* Actor) const
{
	if (!Actor || !AssetManager)
	{
		return false;
	}

	const FGuid MeshId = Item.MeshAssetId.Value.Value;
	if (!MeshId.IsValid())
	{
		return false;
	}

	if (ISpatialMeshItemActorInterface* MeshInterface = Cast<ISpatialMeshItemActorInterface>(Actor))
	{
		if (UStaticMeshComponent* MeshComp = MeshInterface->GetSpatialMeshComponent())
		{
			if (UStaticMesh* MeshAsset = AssetManager->GetCachedStaticMesh(MeshId))
			{
				MeshComp->SetStaticMesh(MeshAsset);
				return true;
			}
		}
	}

	return false;
}

bool AVRegionClient::ApplyMaterialsWork(const FVRegionRenderWorkItem& Item, AActor* Actor) const
{
	if (!Actor || !MaterialPresenter)
	{
		return false;
	}

	if (ISpatialMeshItemActorInterface* MeshInterface = Cast<ISpatialMeshItemActorInterface>(Actor))
	{
		if (UStaticMeshComponent* MeshComp = MeshInterface->GetSpatialMeshComponent())
		{
			if (Item.MaterialIdsBySlot.Num() == 0)
			{
				return false;
			}

			TArray<int32> AllSlots;
			AllSlots.Reserve(Item.MaterialIdsBySlot.Num());
			for (int32 Index = 0; Index < Item.MaterialIdsBySlot.Num(); ++Index)
			{
				AllSlots.Add(Index);
			}

			MaterialPresenter->ApplyMaterialsForSlots(MeshComp, Item.MaterialIdsBySlot, AllSlots);
			return true;
		}
	}

	return false;
}

bool AVRegionClient::ApplyLegacyFallback(
	const FVRegionRenderWorkItem& Item,
	AActor* Actor,
	const UVRegionClientResolver::FRegionClientItemSnapshot& Snapshot)
{
	if (!Actor)
	{
		return false;
	}

	FSpatialActorVisualState LegacyState;
	LegacyState.MeshAssetId = Snapshot.MeshAssetId;
	LegacyState.MaterialIdsBySlot = Snapshot.MaterialIdsBySlot;
	LegacyState.PendingMaterialSlots.Reset();
	LegacyState.PendingMaterialSlots.Reserve(LegacyState.MaterialIdsBySlot.Num());
	for (int32 Index = 0; Index < LegacyState.MaterialIdsBySlot.Num(); ++Index)
	{
		LegacyState.PendingMaterialSlots.Add(Index);
	}
	LegacyState.bForceMaterialReapply = true;
	LegacyState.bMeshPending = true;
	LegacyState.bMaterialsPending = true;

	bool bApplied = false;
	switch (Item.WorkType)
	{
	case EVRegionRenderWorkType::SetMesh:
		bApplied = TryApplyMesh(Item.ItemId, Actor, LegacyState);
		if (bApplied)
		{
			RegionClientResolver->MarkApplied(Item.ItemId, EVRegionRenderWorkType::SetMesh);
		}
		if (bApplied || !LegacyState.MeshAssetId.Value.Value.IsValid())
		{
			if (TryApplyMaterials(Item.ItemId, Actor, LegacyState))
			{
				RegionClientResolver->MarkApplied(Item.ItemId, EVRegionRenderWorkType::ApplyMaterials);
				bApplied = true;
			}
		}
		break;
	case EVRegionRenderWorkType::ApplyMaterials:
		if (TryApplyMaterials(Item.ItemId, Actor, LegacyState))
		{
			RegionClientResolver->MarkApplied(Item.ItemId, EVRegionRenderWorkType::ApplyMaterials);
			bApplied = true;
		}
		break;
	default:
		break;
	}

	return bApplied;
}
