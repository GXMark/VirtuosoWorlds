#include "Region/Spawner/VDecalOverlaySpawner.h"
/*#include "Region/Actor/DecalOverlayActor.h"
#include "Model/Package/MActor.h"

/**
 * Spawn decal actor
 * @param InActorItem = Actor item
 * @param InWorld - World state
 * @return Decal actor
 #1#
ADecalOverlayActor* FDecalOverlaySpawner::SpawnDecalItemActor(const FMActor& InActorItem, UWorld* InWorld)
{
	if (ADecalOverlayActor* ParentActor = InWorld->SpawnActor<ADecalOverlayActor>(ADecalOverlayActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator))
	{
		// Create a root component for the actor
		// USceneComponent* Root = NewObject<USceneComponent>(ParentActor);

		// Location and rotation for spawning
		//ParentActor->SetActorTransform(InActorItem.transform);
		FVector Location(FVector(InActorItem.transform.position));
		FRotator Rotation(InActorItem.transform.rotation.Y, InActorItem.transform.rotation.Z, InActorItem.transform.rotation.X);
		ParentActor->SetActorLocation(Location);
		ParentActor->SetActorRotation(Rotation);

		//Root->RegisterComponent();
		//ParentActor->SetRootComponent(Root);

		/*if (!IsServer)
		{
			TArray<UMaterialInstanceDynamic*> DynamicMaterials;
			AssetCacheManager->DeserializeMaterials({SharedMaterialMap[Component.decal_comp.material_id]}, DynamicMaterials, IsServer);

			UDecalComponent* DecalComponent = UGameplayStatics::SpawnDecalAttached(
				DynamicMaterials[0],
				FVector(Component.decal_comp.size),
				ParentActor->GetRootComponent(),
				NAME_None,
				Location,
				Rotation,
				EAttachLocation::KeepWorldPosition,
				0 // life span in seconds (0 - infinite)
			);

			DecalComponent->SetDecalColor(FLinearColor::Blue);
			DecalComponent->SetFadeScreenSize(Component.decal_comp.fade_screen_size);
			DecalComponent->SetFadeIn(Component.decal_comp.fade_in_start_delay, Component.decal_comp.fade_in_duration);
			DecalComponent->SetFadeOut(Component.decal_comp.fade_out_start_delay, Component.decal_comp.fade_out_duration, Component.decal_comp.destroy_after_fade_out);
		}#1#
		
		//UE_LOG(LogTemp, Log, TEXT("Decal Stream Helper - Spawn Decal Actor [ Status: Success, Position: %s, Rotation: %s ]"),
		//	   *Component.transform.position.ToString(), *Component.transform.rotation.ToString());

		// Wake up for replication
		ParentActor->FlushNetDormancy();
		
		return ParentActor;
	}

	UE_LOG(LogTemp, Error, TEXT("Decal Stream Helper - Spawn Decal Actor [ Status: Actor not created ]"));

	return nullptr;
}*/
