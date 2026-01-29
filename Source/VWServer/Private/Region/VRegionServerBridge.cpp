#include "Region/VRegionServerBridge.h"
#include "Region/VRegionServerBridgeEndpoint.h"

#include "VWDatabase/Public/VDataService.h"

#include "Model/Network/VMSpatialItemNet.h"
#include "Model/Package/VMActor.h"
#include "Model/Constant/VConstants.h"
#include "Model/Package/VMMaterial.h"
#include "Model/Package/VMCollision.h"

#include "JsonObjectConverter.h"

void URegionServerBridge::Initialize(UWorld* InWorld)
{
	World = InWorld;
}

void URegionServerBridge::EnsureSpatialState(APlayerController* PC)
{
	FSpatialStreamState& State = SpatialStreams.FindOrAdd(PC);
	if (State.bInitialized)
	{
		return;
	}

	State.bInitialized = true;
	State.AllItems.Reset();
	State.DeliveredItemIds.Reset();

	if (!World)
	{
		return;
	}

	UVDataService* DataService = UVDataService::Get(World);
	if (!DataService)
	{
		return;
	}

	TArray<FVMActor> ActorItems;
	if (!DataService->GetAllActors(ActorItems))
	{
		return;
	}

	State.AllItems.Reserve(ActorItems.Num());

	auto MakeNetTransform = [](const FVMTransform& In)
	{
		FVMTransformNet Out;
		Out.Location = FVector_NetQuantize10(In.Position);
		const FRotator Rot = FRotator::MakeFromEuler(In.Rotation).GetNormalized();
		Out.Rotation = FVMRotationNet(Rot);
		const bool bHasScale = !In.Scale.Equals(FVector(1.f, 1.f, 1.f), 0.001f);
		Out.bHasScale = bHasScale;
		Out.Scale = bHasScale
			? FVector_NetQuantize10(In.Scale)
			: FVector_NetQuantize10(FVector(1.f, 1.f, 1.f));
		return Out;
	};

	auto MakeMeshNet = [](const FVMMeshComponent& In)
	{
		FVMMeshComponentNet Out;
		Out.mesh_ref = FVMAssetRefNet(In.mesh_ref.id, In.mesh_ref.name, In.mesh_ref.type, In.mesh_ref.path);
		Out.material_ids.Reset(In.material_refs.Num());
		for (const FVMAssetRef& MatRef : In.material_refs)
		{
			Out.material_ids.Add(FVMGuidNet(MatRef.id));
		}
		return Out;
	};

	auto MakePointLightNet = [](const FVMPointLightComponent& In)
	{
		FVMPointLightComponentNet Out;
		Out.intensity = In.intensity;
		Out.units = In.units;
		Out.color = In.color;
		Out.attenuation_radius = In.attenuation_radius;
		Out.source_radius = In.source_radius;
		Out.soft_source_radius = In.soft_source_radius;
		Out.source_length = In.source_length;
		Out.use_temperature = In.use_temperature;
		Out.temperature = In.temperature;
		Out.affects_world = In.affects_world;
		Out.cast_shadows = In.cast_shadows;
		Out.indirect_light_intensity = In.indirect_light_intensity;
		Out.volumetric_scatter_intensity = In.volumetric_scatter_intensity;
		return Out;
	};

	auto MakeSpotLightNet = [](const FVMSpotLightComponent& In)
	{
		FVMSpotLightComponentNet Out;
		Out.intensity = In.intensity;
		Out.units = In.units;
		Out.color = In.color;
		Out.attenuation_radius = In.attenuation_radius;
		Out.source_radius = In.source_radius;
		Out.soft_source_radius = In.soft_source_radius;
		Out.source_length = In.source_length;
		Out.use_temperature = In.use_temperature;
		Out.temperature = In.temperature;
		Out.affects_world = In.affects_world;
		Out.cast_shadows = In.cast_shadows;
		Out.indirect_light_intensity = In.indirect_light_intensity;
		Out.volumetric_scatter_intensity = In.volumetric_scatter_intensity;
		Out.inner_cone_angle = In.inner_cone_angle;
		Out.outer_cone_angle = In.outer_cone_angle;
		return Out;
	};

	auto MakeDecalNet = [](const FVMDecalComponent& In)
	{
		FVMDecalComponentNet Out;
		Out.material_id = FVMGuidNet(In.material_ref.id);
		Out.size = In.size;
		Out.color = FVMLinearColorNet(In.color.name, In.color.color);
		Out.fade_screen_size = In.fade_screen_size;
		Out.fade_in_start_delay = In.fade_in_start_delay;
		Out.fade_in_duration = In.fade_in_duration;
		Out.fade_out_start_delay = In.fade_out_start_delay;
		Out.fade_out_duration = In.fade_out_duration;
		Out.destroy_after_fade_out = In.destroy_after_fade_out;
		return Out;
	};

	for (const FVMActor& A : ActorItems)
	{
		ESpatialItemType PayloadType;
		if (A.type == FVActorType::StaticMeshActor || A.type == FVActorType::HISMActor)
		{
			PayloadType = ESpatialItemType::Mesh;
		}
		else if (A.type == FVActorType::DecalActor)
		{
			PayloadType = ESpatialItemType::Decal;
		}
		else if (A.type == FVActorType::PointLightActor)
		{
			PayloadType = ESpatialItemType::PointLight;
		}
		else if (A.type == FVActorType::SpotLightActor)
		{
			PayloadType = ESpatialItemType::SpotLight;
		}
		else
		{
			continue;
		}

		FVMSpatialItemNet Item;
		Item.ItemID = FVMGuidNet(A.id);
		Item.ParentID = FVMGuidNet(A.pid);
		Item.CollisionID = FVMGuidNet(A.component.collision_id);
		Item.PayloadType = PayloadType;
		Item.Transform = MakeNetTransform(A.transform);

		if (PayloadType == ESpatialItemType::Mesh)
		{
			Item.MeshPayload = MakeMeshNet(A.component.mesh_comp);
		}
		else if (PayloadType == ESpatialItemType::PointLight)
		{
			Item.PointLightPayload = MakePointLightNet(A.component.point_light_comp);
		}
		else if (PayloadType == ESpatialItemType::SpotLight)
		{
			Item.SpotLightPayload = MakeSpotLightNet(A.component.spot_light_comp);
		}
		else if (PayloadType == ESpatialItemType::Decal)
		{
			Item.DecalPayload = MakeDecalNet(A.component.decal_comp);
		}

		State.AllItems.Add(Item);
	}
}

void URegionServerBridge::HandleSpatialRequest(
	APlayerController* PC,
	const FVector& Origin,
	int32 MaxItems)
{
	EnsureSpatialState(PC);

	FSpatialStreamState& State = SpatialStreams.FindOrAdd(PC);

	const int32 SafeMax = FMath::Clamp(MaxItems, 1, 1024);

	// Build candidate list (undelivered)
	TArray<FVMSpatialItemNet> Candidates;
	Candidates.Reserve(State.AllItems.Num());

	for (const FVMSpatialItemNet& Item : State.AllItems)
	{
		const FGuid ItemId = Item.ItemID.Value;

		// Skip invalid IDs so they don't get re-sent forever.
		if (!ItemId.IsValid())
		{
			continue;
		}

		if (!State.DeliveredItemIds.Contains(ItemId))
		{
			Candidates.Add(Item);
		}
	}

	// Sort nearest-first
	Candidates.Sort([&](const FVMSpatialItemNet& A, const FVMSpatialItemNet& B)
	{
		const FVector LA = FVector(A.Transform.Location);
		const FVector LB = FVector(B.Transform.Location);
		return FVector::DistSquared(LA, Origin) < FVector::DistSquared(LB, Origin);
	});

	TArray<FVMSpatialItemNet> Batch;
	Batch.Reserve(SafeMax);

	for (int32 i = 0; i < Candidates.Num() && Batch.Num() < SafeMax; ++i)
	{
		Batch.Add(Candidates[i]);

		const FGuid ItemId = Candidates[i].ItemID.Value;
		if (ItemId.IsValid())
		{
			State.DeliveredItemIds.Add(ItemId);
		}
	}

	const bool bHasMore =
		State.DeliveredItemIds.Num() < State.AllItems.Num();

	if (IVRegionServerBridgeEndpoint* Endpoint = Cast<IVRegionServerBridgeEndpoint>(PC))
	{
		Endpoint->ServerSendSpatialItems(Batch, bHasMore);
	}
}

void URegionServerBridge::HandleMaterialsRequest(
	APlayerController* PC,
	const TArray<FGuid>& MaterialIds)
{
	if (!World)
	{
		return;
	}

	UVDataService* DataService = UVDataService::Get(World);
	if (!DataService)
	{
		return;
	}

	TArray<FVMMaterial> Results;
	Results.Reserve(MaterialIds.Num());

	for (const FGuid& Id : MaterialIds)
	{
		if (!Id.IsValid())
		{
			continue;
		}

		const FString Json = DataService->GetMaterial(Id.ToString());
		if (Json.IsEmpty())
		{
			continue;
		}

		FVMMaterial Mat;
		if (FJsonObjectConverter::JsonObjectStringToUStruct(Json, &Mat, 0, 0))
		{
			Results.Add(Mat);
		}
	}

	if (IVRegionServerBridgeEndpoint* Endpoint = Cast<IVRegionServerBridgeEndpoint>(PC))
	{
		Endpoint->ServerSendMaterialsBatch(Results);
	}
}

void URegionServerBridge::HandleCollisionsRequest(
	APlayerController* PC,
	const TArray<FGuid>& CollisionIds)
{
	if (!World)
	{
		return;
	}

	UVDataService* DataService = UVDataService::Get(World);
	if (!DataService)
	{
		return;
	}

	TArray<FVMCollision> Results;
	Results.Reserve(CollisionIds.Num());

	for (const FGuid& Id : CollisionIds)
	{
		if (!Id.IsValid())
		{
			continue;
		}

		const FString Json = DataService->GetCollision(Id.ToString());
		if (Json.IsEmpty())
		{
			continue;
		}

		FVMCollision Col;
		if (FJsonObjectConverter::JsonObjectStringToUStruct(Json, &Col, 0, 0))
		{
			Results.Add(Col);
		}
	}

	if (IVRegionServerBridgeEndpoint* Endpoint = Cast<IVRegionServerBridgeEndpoint>(PC))
	{
		Endpoint->ServerSendCollisionsBatch(Results);
	}
}
