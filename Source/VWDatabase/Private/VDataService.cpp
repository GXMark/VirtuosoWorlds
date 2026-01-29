#define OBX_CPP_FILE
#define OBX_DLL_IMPORTS
#include "VDataService.h"
#include "JsonObjectConverter.h"
#include "objectbox.hpp"
#include "objectbox-model.h"
#include "vw.obx.hpp"
#include "HAL/FileManager.h"
#include "Misc/Paths.h"
#include "Model/Package/VMPackage.h"
#include "UObject/Object.h"


/**
 * Initialize the database
 */
void UVDataService::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection); // Call base (good practice)
}

/**
 * Initialize the database store
 * @return 
 */
bool UVDataService::InitializeStore()
{
	void* Handle = FPlatformProcess::GetDllHandle(TEXT("objectbox.dll"));
	if (!Handle)
	{
		UE_LOG(LogTemp, Error, TEXT("Database Service - Initialize Store [ Failed To Load objectbox.dll ]"));
	}

	if (!Store.IsValid())
	{
		try
		{
			// Define your model
			OBX_model* model = create_obx_model();

			// Set the custom directory path
			const FString DirectoryPath = FPaths::ProjectSavedDir() + TEXT("Database");
			IFileManager::Get().MakeDirectory(*DirectoryPath, true); // Ensure directory exists
			const std::string StorePath = TCHAR_TO_UTF8(*DirectoryPath);

			// Create store options and set the directory
			obx::Options Options(model);
			Options.directory(StorePath.c_str());

			// Create the store with the options
			Store = MakeUnique<obx::Store>(Options);

			UE_LOG(LogTemp, Log, TEXT("Database Service - Initialize [ Success ]"));
		}
		catch (const obx::Exception& Ex)
		{
			UE_LOG(LogTemp, Error, TEXT("Database Service - Initialize [ Failed, %s ]"), *FString(Ex.what()));
			return false;
		}

		return true;
	}

	UE_LOG(LogTemp, Error, TEXT("Database Service - Initialize [ Failed, Store Invalid ]"));

	return false;
}

void UVDataService::Deinitialize()
{
	Super::Deinitialize();

	if (Store.IsValid())
	{
		Store.Reset();

		UE_LOG(LogTemp, Log, TEXT("Database Service - Deinitialized [ Closed ]"));
	}
}

/*/**
 * Remaps all the package IDs
 *
 * Remaps the package local guids to global guids to protect
 * actor, material identity from client tampering
 *
 * @return true - success, false - failed
 #1#
bool UDataService::RemapPackageIDs(TArray<FMPackage>& OutPackages)
{
	TMap<FGuid, FGuid> LocalToGlobalIDMap;
	FGuid GlobalID;

	for (FMPackage Package : OutPackages)
	{
		for (int32 i = 0; i < Package.actors.Num(); i++)
		{
			if (LocalToGlobalIDMap.Contains(Package.actors[i].id))
			{
				GlobalID = LocalToGlobalIDMap[Package.actors[i].id];
			}
			else
			{
				GlobalID = FGuid::NewGuid();
				LocalToGlobalIDMap.Add(Package.actors[i].id, GlobalID);
			}

			// Replace the actor local material ids with global material ids
			if (Package.actors[i].type == FActorType::StaticMesh)
			{
				TArray<FMAssetRef> MaterialRefs;
				for (FMAssetRef MaterialRef : Package.actors[i].component.static_mesh_comp.material_refs)
				{
					if (LocalToGlobalIDMap.Contains(MaterialRef.id))
					{
						GlobalID = LocalToGlobalIDMap[MaterialRef.id];
					}
					else
					{
						GlobalID = FGuid::NewGuid();
						LocalToGlobalIDMap.Add(MaterialRef.id, GlobalID);
					}

					MaterialRefs.Add(FMAssetRef(GlobalID, MaterialRef.name, FAssetType::Material));
				}

				Package.actors[i].component.static_mesh_comp.material_refs = MaterialRefs;
			}
			else if (Package.actors[i].type == FActorType::HISM)
			{
				TArray<FMAssetRef> MaterialRefs;
				for (FMAssetRef MaterialRef : Package.actors[i].component.hism_comp.material_refs)
				{
					if (LocalToGlobalIDMap.Contains(MaterialRef.id))
					{
						GlobalID = LocalToGlobalIDMap[MaterialRef.id];
					}
					else
					{
						GlobalID = FGuid::NewGuid();
						LocalToGlobalIDMap.Add(MaterialRef.id, GlobalID);
					}

					MaterialRefs.Add(FMAssetRef(GlobalID, MaterialRef.name, FAssetType::Material));
				}

				Package.actors[i].component.hism_comp.material_refs = MaterialRefs;
			}
			else if (Package.actors[i].type == FActorType::Decal)
			{
				FMAssetRef MaterialRef = Package.actors[i].component.decal_comp.material_ref;

				if (LocalToGlobalIDMap.Contains(MaterialRef.id))
				{
					GlobalID = LocalToGlobalIDMap[MaterialRef.id];
				}
				else
				{
					GlobalID = FGuid::NewGuid();
					LocalToGlobalIDMap.Add(MaterialRef.id, GlobalID);
				}

				Package.actors[i].component.decal_comp.material_ref.id = GlobalID;
			}
		}

		// Replace the local material ids with the global material ids
		for (int32 j = 0; j < Package.materials.Num(); j++)
		{
			if (LocalToGlobalIDMap.Contains(Package.materials[j].id))
			{
				Package.materials[j].id = LocalToGlobalIDMap[Package.materials[j].id];
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Database Service - Remap Package IDs [ Material Not Found, Package Name: %s, Materials: %s ]"), *Package.name, *Package.materials[j].id.ToString());
				return false;
			}
		}
	}

	return true;
}*/

/**
 * Add package items to database
 * 
 * @param InPackages - Array of packages
 */
bool UVDataService::AddPackages(const TArray<FVMPackage>& InPackages) const
{
	for (FVMPackage Package : InPackages)
	{
		for (FVMActor Actor : Package.actors)
		{
			if (FString JSON; FJsonObjectConverter::UStructToJsonObjectString(Actor, JSON))
			{
				if (!AddActor(Actor.id.ToString(), JSON))
				{
					UE_LOG(LogTemp, Error, TEXT("Database Service - Add Packages [ Failed To Add Actor ]"));
					return false;
				}
			}
		}

		for (FVMMaterial Material : Package.materials)
		{
			if (FString JSON; FJsonObjectConverter::UStructToJsonObjectString(Material, JSON))
			{
				if (!AddMaterial(Material.id.ToString(), JSON))
				{
					UE_LOG(LogTemp, Error, TEXT("Database Service - Add Packages [ Failed To Add Material ]"));
					return false;
				}
			}
		}

		UE_LOG(LogTemp, Log, TEXT("Database Service - Add Packages [ Package Name: %s, Actors: %d, Materials: %d ]"),
			*Package.name.ToString(), Package.actors.Num(), Package.materials.Num());
	}

	return true;
}

/**
 * Remove all data
 */
bool UVDataService::RemovalAll() const
{
	try
	{
		obx::Box<Actor> ActorBox(*Store);
		ActorBox.removeAll();
		obx::Box<Material> MaterialBox(*Store);
		MaterialBox.removeAll();

		UE_LOG(LogTemp, Log, TEXT("Database Service - RemovalAll [ Success ]"));

		return true;
	}
	catch (const obx::Exception& Ex)
	{
		UE_LOG(LogTemp, Error, TEXT("Database Service - RemovalAll [ Failed, %s ]"), *FString(Ex.what()));
		return false;
	}
}

/**
 * Count actor
 * @return Number of actors
 */
uint64_t UVDataService::ActorCount() const
{
	try
	{
		obx::Box<Actor> ActorBox(*Store);

		const uint64_t ActorCount = ActorBox.count();

		//UE_LOG(LogTemp, Log, TEXT("Database - ActorCount [ Count: %llu ]"), ActorCount);

		return ActorCount;
	}
	catch (const obx::Exception& Ex)
	{
		UE_LOG(LogTemp, Error, TEXT("Database Service - ActorCount [ Count Actor Error, Reason: %s ]"), *FString(Ex.what()));
	}
	return 0;
}

/**
 * Count material
 * @return Number of materials
 */
uint64_t UVDataService::MaterialCount() const
{
	try
	{
		obx::Box<Material> MaterialBox(*Store);

		const uint64_t MaterialCount = MaterialBox.count();

		//UE_LOG(LogTemp, Log, TEXT("Database - MaterialCount [ Material Count: %llu ]"), MaterialCount);

		return MaterialCount;
	}
	catch (const obx::Exception& Ex)
	{
		UE_LOG(LogTemp, Error, TEXT("Database Service - MaterialCount [ Count Material Error, Reason: %s ]"), *FString(Ex.what()));
	}
	return 0;
}


/**
 * Count collisions
 * @return Number of collisions
 */
uint64_t UVDataService::CollisionCount() const
{
	try
	{
		obx::Box<Collision> CollisionBox(*Store);

		const uint64_t CollisionCount = CollisionBox.count();

		//UE_LOG(LogTemp, Log, TEXT("Database - CollisionCount [ Collision Count: %llu ]"), MaterialCount);

		return CollisionCount;
	}
	catch (const obx::Exception& Ex)
	{
		UE_LOG(LogTemp, Error, TEXT("Database Service - CollisionCount [ Count Collision Error, Reason: %s ]"), *FString(Ex.what()));
	}
	return 0;
}

/**
 * Add the actor by key
 * @param InKey - Identifier (guid)
 * @param InValue - Value (json)
 * @return true - Success, false - Fail
 */
bool UVDataService::AddActor(const FString& InKey, const FString& InValue) const
{
	try
	{
		obx::Box<Actor> ActorBox(*Store);
		Actor Actor;
		Actor.id = 0;
		Actor.key = std::string(TCHAR_TO_UTF8(*InKey));
		Actor.value = std::string(TCHAR_TO_UTF8(*InValue));
		const obx_id ID = ActorBox.put(Actor);

		//UE_LOG(LogTemp, Log, TEXT("Database - AddActor [ Add Actor, ID: %llu, Key: %s ]"), ID, *Key);
	}
	catch (const obx::Exception& Ex)
	{
		UE_LOG(LogTemp, Error, TEXT("Database Service - AddActor [ Add Actor Error, Reason: %s ]"), *FString(Ex.what()));
		return false;
	}
	return true;
}

/**
 * Update actor by key
 * @param InKey - Identifier (guid) 
 * @param InValue - Value (json)
 */
void UVDataService::UpdateActor(const FString& InKey, const FString& InValue) const
{
	try
	{
		obx::Box<Actor> ActorBox(*Store);
		auto Query = ActorBox.query(Actor_::key.equals(TCHAR_TO_UTF8(*InKey))).build();

		if (std::unique_ptr<Actor> Entity = Query.findFirst())
		{
			Entity->value = std::string(TCHAR_TO_UTF8(*InValue));
			ActorBox.put(*Entity);

			//UE_LOG(LogTemp, Log, TEXT("Database - UpdateActor [ Key: %s ]"), *Key);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Database Service - UpdateActor [ Update Actor Not Found, Key: %s ]"), *InKey);
		}
	}
	catch (const obx::Exception& Ex)
	{
		UE_LOG(LogTemp, Error, TEXT("Database Service - UpdateActor [ Update Actor Error, Reason: %s ]"), *FString(Ex.what()));
	}
}

/**
 * Update material by key
 * @param InKey - Identifier (guid) 
 * @param InValue - Value (json)
 */
void UVDataService::UpdateMaterial(const FString& InKey, const FString& InValue) const
{
	try
	{
		obx::Box<Material> MaterialBox(*Store);
		auto Query = MaterialBox.query(Actor_::key.equals(TCHAR_TO_UTF8(*InKey))).build();

		if (std::unique_ptr<Material> Entity = Query.findFirst())
		{
			Entity->value = std::string(TCHAR_TO_UTF8(*InValue));
			MaterialBox.put(*Entity);

			//UE_LOG(LogTemp, Log, TEXT("Database - UpdateMaterial [ Key: %s ]"), *Key);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Database Service - UpdateMaterial [ Update Material Not Found, Key: %s ]"), *InKey);
		}
	}
	catch (const obx::Exception& Ex)
	{
		UE_LOG(LogTemp, Error, TEXT("Database Service - UpdateMaterial [ Update Material Error, Reason: %s ]"), *FString(Ex.what()));
	}
}

/**
 * Update collision by key
 * @param InKey - Identifier (guid) 
 * @param InValue - Value (json)
 */
void UVDataService::UpdateCollision(const FString& InKey, const FString& InValue) const
{
	try
	{
		obx::Box<Collision> CollisionBox(*Store);
		auto Query = CollisionBox.query(Actor_::key.equals(TCHAR_TO_UTF8(*InKey))).build();

		if (std::unique_ptr<Collision> Entity = Query.findFirst())
		{
			Entity->value = std::string(TCHAR_TO_UTF8(*InValue));
			CollisionBox.put(*Entity);

			//UE_LOG(LogTemp, Log, TEXT("Database - UpdateCollision [ Key: %s ]"), *Key);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Database Service - UpdateCollision [ Update Collision Not Found, Key: %s ]"), *InKey);
		}
	}
	catch (const obx::Exception& Ex)
	{
		UE_LOG(LogTemp, Error, TEXT("Database Service - UpdateCollision [ Update Collision Error, Reason: %s ]"), *FString(Ex.what()));
	}
}

/**
 * Remove the actor by key
 * @param InKey - key (guid)
 */
void UVDataService::RemoveActor(const FString& InKey) const
{
	try
	{
		obx::Box<Actor> ActorBox(*Store);
		auto Query = ActorBox.query(Actor_::key.equals(TCHAR_TO_UTF8(*InKey))).build();

		if (const std::unique_ptr<Actor> Entity = Query.findFirst())
		{
			ActorBox.remove(Entity->id);

			//UE_LOG(LogTemp, Log, TEXT("Database - RemoveActor [ Key: %s ]"), *Key);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Database Service - RemoveActor [ Remove Actor Not Found, Key: %s ]"), *InKey);
		}
	}
	catch (const obx::Exception& Ex)
	{
		UE_LOG(LogTemp, Error, TEXT("Database Service - RemoveActor [ Remove Actor Error, Reason: %s ]"), *FString(Ex.what()));
	}
}

/**
 * Remove material by key
 * @param InKey - key (guid)
 */
void UVDataService::RemoveMaterial(const FString& InKey) const
{
	try
	{
		obx::Box<Material> MaterialBox(*Store);
		auto Query = MaterialBox.query(Material_::key.equals(TCHAR_TO_UTF8(*InKey))).build();

		if (const std::unique_ptr<Material> Entity = Query.findFirst())
		{
			MaterialBox.remove(Entity->id);

			//UE_LOG(LogTemp, Log, TEXT("Database - RemoveMaterial [ Key: %s ]"), *Key);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Database Service - RemoveMaterial [ Remove Material Not Found, Key: %s ]"), *InKey);
		}
	}
	catch (const obx::Exception& Ex)
	{
		UE_LOG(LogTemp, Error, TEXT("Database Service - RemoveMaterial [ Remove Material Error, Reason: %s ]"), *FString(Ex.what()));
	}
}

/**
 * Remove collision by key
 * @param InKey - key (guid)
 */
void UVDataService::RemoveCollision(const FString& InKey) const
{
	try
	{
		obx::Box<Collision> CollisionBox(*Store);
		auto Query = CollisionBox.query(Collision_::key.equals(TCHAR_TO_UTF8(*InKey))).build();

		if (const std::unique_ptr<Collision> Entity = Query.findFirst())
		{
			CollisionBox.remove(Entity->id);

			//UE_LOG(LogTemp, Log, TEXT("Database - RemoveCollision [ Key: %s ]"), *Key);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Database Service - RemoveCollision [ Remove Collision Not Found, Key: %s ]"), *InKey);
		}
	}
	catch (const obx::Exception& Ex)
	{
		UE_LOG(LogTemp, Error, TEXT("Database Service - RemoveCollision [ Remove Collision Error, Reason: %s ]"), *FString(Ex.what()));
	}
}

/**
 * Add material
 * @param InKey - Identifier (guid) 
 * @param InValue - Value (json)
 * @return true - success, false - failed
 */
bool UVDataService::AddMaterial(const FString& InKey, const FString& InValue) const
{
	try
	{
		obx::Box<Material> MaterialBox(*Store);
		Material Material;
		Material.id = 0;
		Material.key = std::string(TCHAR_TO_UTF8(*InKey));
		Material.value = std::string(TCHAR_TO_UTF8(*InValue));
		const obx_id ID = MaterialBox.put(Material);

		//UE_LOG(LogTemp, Log, TEXT("Database - AddMaterial [ ID: %llu, Key: %s ]"), ID, *Key);
	}
	catch (const obx::Exception& Ex)
	{
		UE_LOG(LogTemp, Error, TEXT("Database Service - AddMaterial [ Add Material Error, Reason: %s ]"), *FString(Ex.what()));
		return false;
	}
	return true;
}

/**
 * Add collision
 * @param InKey - Identifier (guid) 
 * @param InValue - Value (json)
 * @return true - success, false - failed
 */
bool UVDataService::AddCollision(const FString& InKey, const FString& InValue) const
{
	try
	{
		obx::Box<Collision> CollisionBox(*Store);
		Collision Collision;
		Collision.id = 0;
		Collision.key = std::string(TCHAR_TO_UTF8(*InKey));
		Collision.value = std::string(TCHAR_TO_UTF8(*InValue));
		const obx_id ID = CollisionBox.put(Collision);

		//UE_LOG(LogTemp, Log, TEXT("Database - AddCollision [ ID: %llu, Key: %s ]"), ID, *Key);
	}
	catch (const obx::Exception& Ex)
	{
		UE_LOG(LogTemp, Error, TEXT("Database Service - AddCollision [ Add Collision Error, Reason: %s ]"), *FString(Ex.what()));
		return false;
	}
	return true;
}

/**
 * Get the actor
 * @param InKey - Identifier (guid)
 * @return Actor json
 */
FString UVDataService::GetActor(const FString& InKey) const
{
	try
	{
		obx::Box<Actor> ActorBox(*Store);
		auto Query = ActorBox.query(Actor_::key.equals(TCHAR_TO_UTF8(*InKey))).build();
		FString Result = FString(UTF8_TO_TCHAR(Query.findFirst()->value.c_str()));

		//UE_LOG(LogTemp, Log, TEXT("Database - GetActor [ Key: %s ]"), *Key);

		return Result;
	}
	catch (const obx::Exception& Ex)
	{
		UE_LOG(LogTemp, Error, TEXT("Database Service - GetActor [ Get Actor Error, Reason: %s ]"), *FString(Ex.what()));
	}

	return FString();
}

/**
 * Get material
 * @param InKey - Identifier (guid)
 * @return Material json
 */
FString UVDataService::GetMaterial(const FString& InKey) const
{
	try
	{
		obx::Box<Material> MaterialBox(*Store);
		auto Query = MaterialBox.query(Material_::key.equals(TCHAR_TO_UTF8(*InKey))).build();
		FString Result = FString(UTF8_TO_TCHAR(Query.findFirst()->value.c_str()));

		//UE_LOG(LogTemp, Log, TEXT("Database - GetMaterial [ Key: %s ]"), *Key);

		return Result;
	}
	catch (const obx::Exception& Ex)
	{
		UE_LOG(LogTemp, Error, TEXT("Database Service - GetMaterial [ Add Material Error, Reason: %s ]"), *FString(Ex.what()));
	}

	return FString();
}

/**
 * Get collision
 * @param InKey - Identifier (guid)
 * @return Collision json
 */
FString UVDataService::GetCollision(const FString& InKey) const
{
	try
	{
		obx::Box<Collision> CollisionBox(*Store);
		auto Query = CollisionBox.query(Collision_::key.equals(TCHAR_TO_UTF8(*InKey))).build();
		FString Result = FString(UTF8_TO_TCHAR(Query.findFirst()->value.c_str()));

		//UE_LOG(LogTemp, Log, TEXT("Database - GetCollision [ Key: %s ]"), *Key);

		return Result;
	}
	catch (const obx::Exception& Ex)
	{
		UE_LOG(LogTemp, Error, TEXT("Database Service - GetCollision [ Add Collision Error, Reason: %s ]"), *FString(Ex.what()));
	}

	return FString();
}

/**
 * Get all actor items
 * @param OutActorItems - Actor items
 */
bool UVDataService::GetAllActors(TArray<FVMActor>& OutActorItems) const
{
	OutActorItems.Empty();
	try
	{
		obx::Box<Actor> ActorBox(*Store);
		std::vector<std::unique_ptr<Actor>> AllActors = ActorBox.getAll();

		for (const auto& Actor : AllActors)
		{
			FVMActor ActorItem;
			if (FJsonObjectConverter::JsonObjectStringToUStruct<FVMActor>(FString(UTF8_TO_TCHAR(Actor->value.c_str())), &ActorItem, 0, 0))
				OutActorItems.Add(ActorItem);
		}
	}
	catch (const obx::Exception& Ex)
	{
		UE_LOG(LogTemp, Error, TEXT("Database Service - GetAllActors [ Get All Actors Error, Reason: %s ]"), *FString(Ex.what()));
		return false;
	}

	return true;
}

/**
 * Get all material items
 * @return Array of material items
 */
bool UVDataService::GetAllMaterials(TArray<FVMMaterial>& OutMaterialItems) const
{
	OutMaterialItems.Empty();
	try
	{
		TArray<FString> StringMaterials;
		obx::Box<Material> MaterialBox(*Store);
		std::vector<std::unique_ptr<Material>> AllMaterials = MaterialBox.getAll();

		for (const auto& Material : AllMaterials)
		{
			FVMMaterial MaterialItem;
			if (FJsonObjectConverter::JsonObjectStringToUStruct<FVMMaterial>(FString(UTF8_TO_TCHAR(Material->value.c_str())), &MaterialItem, 0, 0))
				OutMaterialItems.Add(MaterialItem);
		}
	}
	catch (const obx::Exception& Ex)
	{
		UE_LOG(LogTemp, Error, TEXT("Database Service - GetAllMaterials [ Get All Materials Error, Reason: %s ]"), *FString(Ex.what()));
		return false;
	}

	return true;
}

/**
 * Get all collision items
 * @return Array of collision items
 */
bool UVDataService::GetAllCollisions(TArray<FVMCollision>& OutCollisionItems) const
{
	OutCollisionItems.Empty();
	try
	{
		TArray<FString> StringCollisions;
		obx::Box<Collision> CollisionBox(*Store);
		std::vector<std::unique_ptr<Collision>> AllCollisions = CollisionBox.getAll();

		for (const auto& Collision : AllCollisions)
		{
			FVMCollision CollisionItem;
			if (FJsonObjectConverter::JsonObjectStringToUStruct<FVMCollision>(FString(UTF8_TO_TCHAR(Collision->value.c_str())), &CollisionItem, 0, 0))
				OutCollisionItems.Add(CollisionItem);
		}
	}
	catch (const obx::Exception& Ex)
	{
		UE_LOG(LogTemp, Error, TEXT("Database Service - GetAllCollisions [ Get All Collisions Error, Reason: %s ]"), *FString(Ex.what()));
		return false;
	}

	return true;
}
