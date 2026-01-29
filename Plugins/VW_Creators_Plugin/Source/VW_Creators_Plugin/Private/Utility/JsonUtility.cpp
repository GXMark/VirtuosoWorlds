#include "Model/Utility/JsonUtility.h"
#include "JsonObjectConverter.h"
#include "Constant/Constants.h"
#include "Model/Package/MPPackage.h"

FString FJsonUtility::ToJsonString(const FMPPackage& Package)
{
	// Convert whole struct first
	TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();;
	FJsonObjectConverter::UStructToJsonObject(FMPPackage::StaticStruct(), &Package, JsonObject.ToSharedRef());

	// Get actors
	const TArray<TSharedPtr<FJsonValue>>* ActorsArray;
	if (JsonObject->TryGetArrayField(TEXT("actors"), ActorsArray))
	{
		for (const TSharedPtr<FJsonValue>& ActorValue : *ActorsArray)
		{
			if (!ActorValue.IsValid() || ActorValue->Type != EJson::Object)
				continue;

			TSharedPtr<FJsonObject> ActorObj = ActorValue->AsObject();
			if (!ActorObj.IsValid())
				continue;

			FString ActorType;
			ActorObj->TryGetStringField(TEXT("type"), ActorType);

			// Now use CompObj normally
			if (ActorType == FPTargetType::StaticMeshActor)
			{
				ActorObj->RemoveField(TEXT("hism_comp"));
				ActorObj->RemoveField(TEXT("decal_comp"));
				ActorObj->RemoveField(TEXT("point_light_comp"));
				ActorObj->RemoveField(TEXT("spot_light_comp"));
			}
			else if (ActorType == FPTargetType::HISMActor)
			{
				ActorObj->RemoveField(TEXT("static_mesh_comp"));
				ActorObj->RemoveField(TEXT("decal_comp"));
				ActorObj->RemoveField(TEXT("point_light_comp"));
				ActorObj->RemoveField(TEXT("spot_light_comp"));
			}
			else if (ActorType == FPTargetType::DecalActor)
			{
				ActorObj->RemoveField(TEXT("static_mesh_comp"));
				ActorObj->RemoveField(TEXT("hism_comp"));
				ActorObj->RemoveField(TEXT("point_light_comp"));
				ActorObj->RemoveField(TEXT("spot_light_comp"));
			}
			else if (ActorType == FPTargetType::PointLightActor)
			{
				ActorObj->RemoveField(TEXT("static_mesh_comp"));
				ActorObj->RemoveField(TEXT("hism_comp"));
				ActorObj->RemoveField(TEXT("decal_comp"));
				ActorObj->RemoveField(TEXT("spot_light_comp"));
			}
			else if (ActorType == FPTargetType::SpotLightActor)
			{
				ActorObj->RemoveField(TEXT("static_mesh_comp"));
				ActorObj->RemoveField(TEXT("hism_comp"));
				ActorObj->RemoveField(TEXT("decal_comp"));
				ActorObj->RemoveField(TEXT("point_light_comp"));
			}
			else if (ActorType == FPTargetType::SceneActor)
			{
				ActorObj->RemoveField(TEXT("static_mesh_comp"));
				ActorObj->RemoveField(TEXT("hism_comp"));
				ActorObj->RemoveField(TEXT("decal_comp"));
				ActorObj->RemoveField(TEXT("point_light_comp"));
				ActorObj->RemoveField(TEXT("spot_light_comp"));
			}
		}
	}

	// Get collisions
	const TArray<TSharedPtr<FJsonValue>>* CollisionArray;
	if (JsonObject->TryGetArrayField(TEXT("collisions"), CollisionArray))
	{
		for (const TSharedPtr<FJsonValue>& CollisionValue : *CollisionArray)
		{
			TSharedPtr<FJsonObject> CollisionObj = CollisionValue->AsObject();
			if (!CollisionObj.IsValid())
				continue;

			const TArray<TSharedPtr<FJsonValue>>* SphereArray;
			if (CollisionObj->TryGetArrayField(TEXT("spheres"), SphereArray))
			{
				if (SphereArray->IsEmpty())
					CollisionObj->RemoveField(TEXT("spheres"));
			}
			const TArray<TSharedPtr<FJsonValue>>* BoxArray;
			if (CollisionObj->TryGetArrayField(TEXT("boxes"), BoxArray))
			{
				if (BoxArray->IsEmpty())
					CollisionObj->RemoveField(TEXT("boxes"));
			}
			const TArray<TSharedPtr<FJsonValue>>* CapsuleArray;
			if (CollisionObj->TryGetArrayField(TEXT("capsules"), CapsuleArray))
			{
				if (CapsuleArray->IsEmpty())
					CollisionObj->RemoveField(TEXT("capsules"));
			}
			const TArray<TSharedPtr<FJsonValue>>* ConvexArray;
			if (CollisionObj->TryGetArrayField(TEXT("convexes"), ConvexArray))
			{
				if (ConvexArray->IsEmpty())
					CollisionObj->RemoveField(TEXT("convexes"));
			}
		}
	}

	// Re-serialize
	FString OutString;
	auto Writer = TJsonWriterFactory<>::Create(&OutString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
	return OutString;
}
