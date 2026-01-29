#include "Region/Spawner/VMeshSpawner.h"
/*#include "Model/Package/MActor.h"
#include "Engine/Engine.h"
#include "Engine/StaticMesh.h"
#include "Region/Actor/HISMActor.h"
#include "Region/Actor/MeshActor.h"
#include "Region/Actor/SceneActor.h"

/**
 * Spawn scene actor
 * @param InActorItem - Actor item
 * @param InWorld - World state
 #1#
ASceneActor* FMeshSpawner::SpawnSceneActor(const FMActor& InActorItem, UWorld* InWorld)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = nullptr;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	if (ASceneActor* SceneActor = InWorld->SpawnActor<ASceneActor>(ASceneActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator))
	{
		// Move the actor to the desired location and rotation
		FVector Location(FVector(InActorItem.transform.position));
		FRotator Rotation(InActorItem.transform.rotation.Y, InActorItem.transform.rotation.Z, InActorItem.transform.rotation.X);
		FVector Scale(InActorItem.transform.scale.X, InActorItem.transform.scale.Y, InActorItem.transform.scale.Z);
		SceneActor->SetActorLocation(Location);
		SceneActor->SetActorRotation(Rotation);
		SceneActor->SetActorScale3D(Scale);
		
		SceneActor->GetRootComponent()->SetMobility(EComponentMobility::Movable);

		// Wake up for replication
		SceneActor->FlushNetDormancy();
		
		return SceneActor;
	}


	UE_LOG(LogTemp, Error, TEXT("Mesh Stream Helper - Spawn Scene Actor [ Status: Actor not created ]"));

	return nullptr;
}

/**
 * Spawn static mesh actor
 * @param InActorItem - Actor item
 * @param InWorld - World state
 * @param InMesh - Mesh asset
 * @return Static mesh actor
 #1#
AMeshActor* FMeshSpawner::SpawnStaticMeshActor(const FMActor& InActorItem, UWorld* InWorld, UStaticMesh* InMesh)
{
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// Spawn an actor of type AStaticMeshActor
	if (AMeshActor* MeshActor = InWorld->SpawnActor<AMeshActor>(AMeshActor::StaticClass(), FTransform::Identity, Params))
	{
		//MeshActor->Item = InActorItem.component.static_mesh_comp;
	
		MeshActor->Component->SetStaticMesh(InMesh);
	
		MeshActor->SetActorLocation(InActorItem.transform.position);
		FRotator Rotator = FRotator(InActorItem.transform.rotation.Y, InActorItem.transform.rotation.Z, InActorItem.transform.rotation.X);
		MeshActor->SetActorRotation(Rotator);
		MeshActor->SetActorScale3D(InActorItem.transform.scale);
		
		return MeshActor;
	}

	UE_LOG(LogTemp, Error, TEXT("Mesh Stream Helper - Spawn Static Mesh Actor [ Status: Actor not created ]"));
	return nullptr;
}

/**
 * Spawn hierarchical instance static mesh (HISM) actor
 * @param InActorItem - Actor item
 * @param InWorld - World state
 * @param InMesh - Mesh asset
 * @return HISM Actor
 #1#
AHISMActor* FMeshSpawner::SpawnHISMActor(const FMActor& InActorItem, UWorld* InWorld, UStaticMesh* InMesh)
{
	//if (UStaticMesh* Mesh = AssetCacheManager->GetMesh(Component.hism_comp.asset_ref.id))
	if (InMesh)
	{
		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		
		if (AHISMActor* HISMActor = InWorld->SpawnActor<AHISMActor>(AHISMActor::StaticClass(), FTransform::Identity, Params))
		{
			HISMActor->Item = InActorItem.component.hism_comp;
	
			// Move the actor
			HISMActor->SetActorLocation(InActorItem.transform.position);
			FRotator Rotator = FRotator(InActorItem.transform.rotation.Y, InActorItem.transform.rotation.Z, InActorItem.transform.rotation.X);
			HISMActor->SetActorRotation(Rotator);
			HISMActor->SetActorScale3D(InActorItem.transform.scale);
			
			// Create the HISM component
			UHierarchicalInstancedStaticMeshComponent* HISMComp = NewObject<UHierarchicalInstancedStaticMeshComponent>(HISMActor);
			HISMComp->RegisterComponent();
			HISMComp->AttachToComponent(HISMActor->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
			HISMComp->SetMobility(EComponentMobility::Movable);
			HISMComp->SetStaticMesh(InMesh);
			
			for (FMTransform TransformItem : InActorItem.component.hism_comp.instance_transforms)
			{
				FTransform LocalTransform = FTransform(FRotator(TransformItem.rotation.Y, TransformItem.rotation.Z, TransformItem.rotation.X), TransformItem.position, TransformItem.scale);
				HISMComp->AddInstance(LocalTransform);
			}
			
			return HISMActor;
		}

		UE_LOG(LogTemp, Error, TEXT("Mesh Stream Helper - Spawn HISM Actor [ Status: Actor not created ]"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Mesh Stream Helper - Spawn HISM Actor [ Status: Mesh not found ]"));
	}

	return nullptr;
}*/
