#include "Region/VRegionClient.h"
#include "Presentation/VMaterialPresenter.h"
#include "Presentation/VMeshPresenter.h"
#include "Subsystem/VAssetManager.h"
#include "Region/VRegionPresenter.h"
#include "Region/VRegionResolver.h"
#include "Region/VRegionClientBridge.h"
#include "Components/SceneComponent.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

AVRegionClient::AVRegionClient()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.TickInterval = 0.f;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
}

void AVRegionClient::BeginPlay()
{
	Super::BeginPlay();

	if (GetNetMode() != NM_Client)
	{
		return;
	}

	PresentationRoot = NewObject<USceneComponent>(this, TEXT("PresentationRoot"));
	PresentationRoot->RegisterComponent();
	PresentationRoot->AttachToComponent(
		Root,
		FAttachmentTransformRules::KeepRelativeTransform);

	AssetManager = GetWorld()->GetSubsystem<UVAssetManager>();

	MeshPresenter = NewObject<UVMeshPresenter>(this);
	MeshPresenter->Initialize(this, PresentationRoot, AssetManager);

	MaterialPresenter = NewObject<UVMaterialPresenter>(this);
	MaterialPresenter->Initialize();

	// Initialize region bridge for all request types (spatial/material).
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		RegionBridge = NewObject<URegionClientBridge>(this, TEXT("RegionBridge"));
		RegionBridge->Initialize(PC);
	}

	RegionResolver = NewObject<UVRegionResolver>(this);
	RegionResolver->Initialize(RegionBridge, AssetManager);

	RegionPresenter = NewObject<UVRegionPresenter>(this);
	RegionPresenter->Initialize(
		MeshPresenter,
		MaterialPresenter);
}

void AVRegionClient::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	(void)DeltaSeconds;

	if (GetNetMode() != NM_Client)
	{
		return;
	}

	if (RegionResolver)
	{
		for (const TArray<FVMSpatialItemNet>& Batch : PendingSpatialBatches)
		{
			RegionResolver->OnSpatialBatchReceived(Batch);
		}
		PendingSpatialBatches.Reset();

		for (const TArray<FVMMaterial>& Batch : PendingMaterialBatches)
		{
			RegionResolver->OnMaterialsBatchReceived(Batch);
		}
		PendingMaterialBatches.Reset();

	}

	if (RegionResolver && RegionPresenter)
	{
		TArray<FResolvedItemBundle> Bundles;
		RegionResolver->GetResolvedBundles(MaxResolvedBundlesPerTick, Bundles);
		RegionPresenter->Commit(Bundles);
	}
}

void AVRegionClient::OnSpatialBatchReceived(const TArray<FVMSpatialItemNet>& Items, bool bHasMore)
{
	(void)bHasMore;
	PendingSpatialBatches.Add(Items);
}

void AVRegionClient::OnMaterialsBatchReceived(const TArray<FVMMaterial>& Materials)
{
	PendingMaterialBatches.Add(Materials);
}
