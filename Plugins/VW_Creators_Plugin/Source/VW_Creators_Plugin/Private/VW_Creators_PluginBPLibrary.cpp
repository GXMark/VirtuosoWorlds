// Copyright Epic Games, Inc. All Rights Reserved.

#include "VW_Creators_PluginBPLibrary.h"
#include "Helper/CacheHelper.h"
#include "Helper/MaterialHelper.h"
#include "Helper/MeshHelper.h"
#include "JsonObjectConverter.h"
#include "Components/DecalComponent.h"
#include "Components/BillboardComponent.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Components/SpotLightComponent.h"
#include "Helper/LightHelper.h"
#include "Misc/FileHelper.h"
#include "Model/Utility/JsonUtility.h"
#include "Utility/ArchiveUtility.h"
#include "Utility/PAKUtility.h"
#include "PhysicsEngine/BodySetup.h"
#include "StaticMeshEditorSubsystem.h"
#include "Constant/Constants.h"
#include "Helper/CollisionHelper.h"
#include "Helper/StatsHelper.h"
#include "PhysicsEngine/PhysicsSettings.h"

FMPPackage UVW_Creators_PluginBPLibrary::Package = {};
FString UVW_Creators_PluginBPLibrary::PluginINI = "VW_Plugin.ini";
TArray<AActor*> UVW_Creators_PluginBPLibrary::Actors = {};
TMap<int32, FGuid> UVW_Creators_PluginBPLibrary::ActorIDMap = {};
bool UVW_Creators_PluginBPLibrary::CancelBuild = false;
FString UVW_Creators_PluginBPLibrary::AssetPath = TEXT("");
bool UVW_Creators_PluginBPLibrary::ClearLogs = false;
bool UVW_Creators_PluginBPLibrary::ShowLogs = false;
bool UVW_Creators_PluginBPLibrary::ShowJson = false;
bool UVW_Creators_PluginBPLibrary::DontPackageAssets = false;
FString UVW_Creators_PluginBPLibrary::PackageName = TEXT("");
FString UVW_Creators_PluginBPLibrary::PackageDescription = TEXT("");
FString UVW_Creators_PluginBPLibrary::PackageAuthor = TEXT("");
FString UVW_Creators_PluginBPLibrary::PackagePath = TEXT("");
bool UVW_Creators_PluginBPLibrary::ClearAllAssets = false;

UVW_Creators_PluginBPLibrary::UVW_Creators_PluginBPLibrary(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}


void UVW_Creators_PluginBPLibrary::CookCurrentLevel()
{
	UE_LOG(LogTemp, Log, TEXT("VWCP - [ Cooking Current Level - Please Wait (Batch File) ... ]"));
	FCPPAKUtility::CookCurrentLevel();
}

void UVW_Creators_PluginBPLibrary::CreatePackageArchive(const TArray<AActor*>& SelectedActors)
{
#if WITH_EDITOR
	if (ClearLogs)
	{
		GEngine->Exec(nullptr, TEXT("Log Clear"));
	}
#endif

	// Temporarily disable annoying log entries
	LogTexture.SetVerbosity(ELogVerbosity::NoLogging);

	// Initialize Package
	Package.actors.Empty();
	Package.materials.Empty();
	Package.engine = FName(FString::Printf(TEXT("%d.%d.%d"), ENGINE_MAJOR_VERSION, ENGINE_MINOR_VERSION, ENGINE_PATCH_VERSION));
	Package.name = FName(PackageName);
	Package.description = FName(PackageDescription);
	Package.author = FName(PackageAuthor);
	Package.created = FName(FDateTime::UtcNow().ToString());

	AssetPath = FPaths::ConvertRelativePathToFull(FPaths::ProjectSavedDir() + TEXT("Assets"));

	if (ShowLogs)
	{
		UE_LOG(LogTemp, Log, TEXT(
			       R"(         
██    ██ ██ ██████  ████████ ██    ██  ██████  ███████  ██████      ██████   █████   ██████ ██   ██  █████   ██████  ███████      ██████ ██████  ███████  █████  ████████  ██████  ██████  
██    ██ ██ ██   ██    ██    ██    ██ ██    ██ ██      ██    ██     ██   ██ ██   ██ ██      ██  ██  ██   ██ ██       ██          ██      ██   ██ ██      ██   ██    ██    ██    ██ ██   ██ 
██    ██ ██ ██████     ██    ██    ██ ██    ██ ███████ ██    ██     ██████  ███████ ██      █████   ███████ ██   ███ █████       ██      ██████  █████   ███████    ██    ██    ██ ██████  
 ██  ██  ██ ██   ██    ██    ██    ██ ██    ██      ██ ██    ██     ██      ██   ██ ██      ██  ██  ██   ██ ██    ██ ██          ██      ██   ██ ██      ██   ██    ██    ██    ██ ██   ██ 
  ████   ██ ██   ██    ██     ██████   ██████  ███████  ██████      ██      ██   ██  ██████ ██   ██ ██   ██  ██████  ███████      ██████ ██   ██ ███████ ██   ██    ██     ██████  ██   ██)"));
		UE_LOG(LogTemp, Log, TEXT(""));
		UE_LOG(LogTemp, Log, TEXT("Plugin Version: 1.0"));
		UE_LOG(LogTemp, Log, TEXT("Unreal Engine Version: %s"), *Package.engine.ToString());
		UE_LOG(LogTemp, Log, TEXT("Name : %s"), *Package.name.ToString());
		UE_LOG(LogTemp, Log, TEXT("Description: %s"), *Package.description.ToString());
		UE_LOG(LogTemp, Log, TEXT("Creator: %s"), *Package.author.ToString());
		UE_LOG(LogTemp, Log, TEXT("Created Date: %s"), *Package.created.ToString());
		UE_LOG(LogTemp, Log, TEXT(""));
		UE_LOG(LogTemp, Log, TEXT("Asset Path: %s"), *AssetPath);
		UE_LOG(LogTemp, Log, TEXT("Package Path: %s"), *PackagePath);
		UE_LOG(LogTemp, Log, TEXT("Show Logs: %s"), ShowLogs ? TEXT("true"):TEXT("false"));
		UE_LOG(LogTemp, Log, TEXT("Show Json: %s"), ShowJson ? TEXT("true"):TEXT("false"));
		UE_LOG(LogTemp, Log, TEXT("Clear All Assets: %s"), ClearAllAssets ? TEXT("true"):TEXT("false"));
		UE_LOG(LogTemp, Log, TEXT("Build Package Archive: %s"), DontPackageAssets ? TEXT("true"):TEXT("false"));
		UE_LOG(LogTemp, Log, TEXT("------------------------------------------------------------------------------"));
	}

	CancelBuild = false;

	FCPStatsHelper::Initialize();

	Actors = SelectedActors;

	// Check the package path

	// Get the platform file manager
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	if (!PlatformFile.DirectoryExists(*PackagePath))
	{
		if (PlatformFile.CreateDirectoryTree(*PackagePath))
		{
			UE_LOG(LogTemp, Log, TEXT("VWCP - [ Package path created ]"));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("VWCP - [ Package path error (Abort) ]"));
			return;
		}
	}

	if (FPCacheHelper::Initialize(AssetPath, ClearAllAssets, ShowLogs))
	{
		if (ShowLogs)
		{
			UE_LOG(LogTemp, Log, TEXT("VWCP - [ Cache Initialized ]"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("VWCP - [ Cache Not Initialized (Abort) ]"));
		return;
	}

	if (FCPMaterialHelper::Initialize(ShowLogs))
	{
		if (ShowLogs)
		{
			UE_LOG(LogTemp, Log, TEXT("VWCP - [ Material Helper Initialized ]"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("VWCP - [ Material Helper Failed To Initialize (Abort) ]"));
		return;
	}

	if (FCPCollisionHelper::Initialize(ShowLogs))
	{
		if (ShowLogs)
		{
			UE_LOG(LogTemp, Log, TEXT("VWCP - [ Collision Helper Initialized ]"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("VWCP - [ Collision Helper Failed To Initialize (Abort) ]"));
		return;
	}


	// Event timer execution state machine
	FTSTicker::GetCoreTicker().AddTicker(
		FTickerDelegate::CreateLambda([SelectedActors](float) -> bool
		{
			static int32 State = 0;
			static int32 Index = 0;
			static FString JSON;

			if (CancelBuild)
			{
				// Reset variables
				State = 0;
				Index = 0;

				UE_LOG(LogTemp, Log, TEXT("VWCP - [ Build Cancelled ]"));

				return false;
			}

			switch (State)
			{
			case 0:
				{
					if (ValidateMaterials(SelectedActors))
					{
						UE_LOG(LogTemp, Log, TEXT("VWCP - [ Material Validation Success ]"));
						State = 1;
					}
					else
					{
						UE_LOG(LogTemp, Log, TEXT("VWCP - [ Material Validation Failed (Abort) ]"));
						State = 7;
					}
				}
				break;
			case 1:
				{
					if (ValidateMesh(SelectedActors))
					{
						UE_LOG(LogTemp, Log, TEXT("VWCP - [ Mesh & Collider Validation Success ]"));
						State = 2;
					}
					else
					{
						UE_LOG(LogTemp, Log, TEXT("VWCP - [ Mesh & Collider Validation Failed (Abort) ]"));
						State = 7;
					}
				}
				break;

			case 2:
				{
					if (Index == 0)
						UE_LOG(LogTemp, Log, TEXT("VWCP - [ Build Package ]"));

					if (Index < Actors.Num())
					{
						BuildPackage(Actors[Index]);
						Index++;
					}
					else
					{
						// Assign materials and collision items to the package
						Package.materials = FCPMaterialHelper::GetMaterialItems();
						Package.collisions = FCPCollisionHelper::GetCollisionItems();

						Index = 0;

						State = 3;
					}
				}
				break;

			case 3:
				if (Index < FPCacheHelper::GetAssetRefs().Num())
				{
					if (Index == 0)
					{
						UE_LOG(LogTemp, Log, TEXT("VWCP - [ Serialize Mesh Assets ]"));
					}
					
					SerializeAsset(FPCacheHelper::GetAssetRefs()[Index]);
								
					// Needed to force the cancel
					if (CancelBuild)
					{
						// Reset variables
						State = 0;
						Index = 0;

						UE_LOG(LogTemp, Log, TEXT("VWCP - [ Build Cancelled ]"));

						return false;
					}

					Index++;
				}
				else
					State = 4;
				break;

			case 4:
				{
					UE_LOG(LogTemp, Log, TEXT("VWCP - [ Save Package ]"));
					Package.id = FGuid::NewGuid();
					FPCacheHelper::AddAssetRef(FMPAssetRef(Package.id, Package.name, FPAssetType::Package, NAME_None, NAME_None));

					JSON = FJsonUtility::ToJsonString(Package);

					FPCacheHelper::SavePackage(Package.id, JSON);

					if (ShowJson)
						UE_LOG(LogTemp, Log, TEXT("%s"), *JSON);

					State = 5;
				}
				break;

			case 5:
				{
					if (DontPackageAssets)
					{
						UE_LOG(LogTemp, Log, TEXT("VWCP - [ Building Archive ]"));
						BuildPackageArchive();
					}

					State = 6;
				}
				break;

			case 6:
				{
					// Reset variables
					State = 0;
					Index = 0;

					FCPStatsHelper::TotalMeshes = FPCacheHelper::GetAssetRefsByType(FPAssetType::Mesh).Num();
					FCPStatsHelper::TotalMaterials = FCPMaterialHelper::GetMaterialItems().Num();
					FCPStatsHelper::TotalTextures = FPCacheHelper::GetAssetRefsByType(FPAssetType::Texture).Num();

					if (ShowLogs)
						FCPStatsHelper::Summary();

					UE_LOG(LogTemp, Log, TEXT("VWCP - [ Package Build Completed: %s ]"), *Package.id.ToString());

					return false;
				}

			case 7:
				{
					// Reset variables
					State = 0;
					Index = 0;

					UE_LOG(LogTemp, Log, TEXT("VWCP - [ Package Build Aborted ]"));

					return false;
				}

			default: ;
			}

			return true;
		}),
		0.01f
	); // Tick every 0.01 seconds
}

// Cancel the package build
void UVW_Creators_PluginBPLibrary::CancelPackageBuild()
{
	CancelBuild = true;
}

// Validate actor materials by check the parents
bool UVW_Creators_PluginBPLibrary::ValidateMaterials(const TArray<AActor*>& SelectedActors)
{
	bool MaterialsValid = true;

	// Iterate through the selected actors
	for (AActor* Actor : SelectedActors)
	{
		if (UStaticMeshComponent* MeshComponent = Actor->FindComponentByClass<UStaticMeshComponent>())
		{
			if (!FCPMaterialHelper::ValidateMaterials(MeshComponent->GetMaterials()))
				MaterialsValid = false;
		}

		if (UHierarchicalInstancedStaticMeshComponent* HISMComponent = Actor->FindComponentByClass<UHierarchicalInstancedStaticMeshComponent>())
		{
			if (!FCPMaterialHelper::ValidateMaterials(HISMComponent->GetMaterials()))
				MaterialsValid = false;
		}

		if (UDecalComponent* DecalComponent = Actor->FindComponentByClass<UDecalComponent>())
		{
			if (!FCPMaterialHelper::ValidateMaterials({DecalComponent->GetDecalMaterial()}))
			{
				MaterialsValid = false;
			}
		}
	}

	return MaterialsValid;
}

// Validate that the mesh colliders are not complex
bool UVW_Creators_PluginBPLibrary::ValidateMesh(const TArray<AActor*>& SelectedActors)
{
	bool ActorValid = true;

	// Iterate through the selected actors
	for (AActor* Actor : SelectedActors)
	{
		UStaticMesh* Mesh = nullptr;

		UStaticMeshComponent* MeshComponent = Actor->FindComponentByClass<UStaticMeshComponent>();
		if (MeshComponent)
		{
			Mesh = MeshComponent->GetStaticMesh();
		}

		UHierarchicalInstancedStaticMeshComponent* HISMComponent = Actor->FindComponentByClass<UHierarchicalInstancedStaticMeshComponent>();
		if (HISMComponent)
		{
			Mesh = HISMComponent->GetStaticMesh();
		}

		// Bypass non-mesh actors
		if (!MeshComponent && !HISMComponent)
			continue;

		if (Mesh)
		{
			// Check its collision type
			ECollisionType CollisionType = DetectCollisionType(Mesh);

			if (CollisionType == ECollisionType::Complex)
			{
				ActorValid = false;
				UE_LOG(LogTemp, Warning, TEXT("VWCP - [ Complex Collider (Change to simple collider), Actor Name: %s, Mesh Name: %s ]"), *Actor->GetActorLabel(), *Mesh->GetName());
			}
			else if (CollisionType == ECollisionType::None)
			{
				ActorValid = false;
				UE_LOG(LogTemp, Warning, TEXT("VWCP - [ No Collider (Add a simple collider), Actor Name: %s, Mesh Name: %s ]"), *Actor->GetActorLabel(), *Mesh->GetName());
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("VWCP - [ Mesh Missing, Actor Name: %s ]"), *Actor->GetActorLabel());
			ActorValid = false;
		}
	}

	return ActorValid;
}

UVW_Creators_PluginBPLibrary::ECollisionType UVW_Creators_PluginBPLibrary::DetectCollisionType(const UStaticMesh* Mesh)
{
	if (!Mesh)
		return ECollisionType::None;

	// --- Check for simple collision primitives ---
	const UBodySetup* Body = Mesh->GetBodySetup();
	const bool bHasSimple =
		Body &&
		(Body->AggGeom.BoxElems.Num() > 0 ||
			Body->AggGeom.SphereElems.Num() > 0 ||
			Body->AggGeom.SphylElems.Num() > 0 ||
			Body->AggGeom.ConvexElems.Num() > 0);

	if (bHasSimple)
		return ECollisionType::Simple;

	// --- No simple primitives → check complex collision ---
	UStaticMeshEditorSubsystem* Subsystem =
		GEditor->GetEditorSubsystem<UStaticMeshEditorSubsystem>();

	if (!Subsystem)
		return ECollisionType::None;

	ECollisionTraceFlag Complexity =
		Subsystem->GetCollisionComplexity(Mesh);

	// Explicit complex
	if (Complexity == CTF_UseComplexAsSimple)
		return ECollisionType::Complex;

	// Mixed mode → still uses complex for traces
	if (Complexity == CTF_UseSimpleAndComplex)
		return ECollisionType::Complex;

	// Explicit simple but no primitives → no collision
	if (Complexity == CTF_UseSimpleAsComplex)
		return ECollisionType::None;

	// Default mode → resolve using project settings
	if (Complexity == CTF_UseDefault)
	{
		const UPhysicsSettings* Phys = GetDefault<UPhysicsSettings>();
		ECollisionTraceFlag ProjectDefault = Phys->DefaultShapeComplexity;

		if (ProjectDefault == CTF_UseComplexAsSimple ||
		ProjectDefault == CTF_UseSimpleAndComplex)
		{
			return ECollisionType::Complex;
		}

		// Default = simple but no primitives → no collision
		return ECollisionType::None;
	}

	return ECollisionType::None;
}

/**
 * Build package archive
 */
void UVW_Creators_PluginBPLibrary::BuildPackageArchive()
{
	// Get an array of absolute asset file paths
	const TArray<FString> AssetRefs = FPCacheHelper::GetAssetPaths();
	const FString PackageArchiveFilePath = FPaths::Combine(PackagePath, PackageName + TEXT(".parchive"));
	FCPArchiveUtility::CreatePackageArchive(AssetRefs, PackageArchiveFilePath);
}

/**
 * Build package
 */
void UVW_Creators_PluginBPLibrary::BuildPackage(AActor* InActor)
{
	ActorIDMap.Empty();

	// Only process root actors
	if (!InActor->GetAttachParentActor())
	{
		// Blueprint (Not Level Blueprint) - Flattened hierarchy to actor items
		if (Cast<UBlueprintGeneratedClass>(InActor->GetClass()))
		{
			TArray<FMPActor> ActorItems;
			FlattenBlueprintActor(InActor, ActorItems);

			Package.actors.Append(ActorItems);

			FCPStatsHelper::TotalActors += ActorItems.Num();

			if (ShowLogs)
				UE_LOG(LogTemp, Log, TEXT("VWCP - [ Blueprint Root Actor: %s,  Actor Items: %d ]"), *InActor->GetActorLabel(), ActorItems.Num());
		}
		else
		// HISM - Flatten instance transforms to actor items 
			if (UHierarchicalInstancedStaticMeshComponent* HISMComponent = InActor->FindComponentByClass<UHierarchicalInstancedStaticMeshComponent>())
			{
				// Materials
				TArray<UMaterialInterface*> Materials;
				Materials = HISMComponent->GetMaterials();

				TArray<FMPAssetRef> MaterialRefs;

				for (int32 MaterialIndex = 0; MaterialIndex < Materials.Num(); MaterialIndex++)
				{
					FMPMaterial MaterialItem;
					if (!FCPMaterialHelper::GetMaterialItem(Materials[MaterialIndex], MaterialItem))
					{
						// Serialize the material
						FCPMaterialHelper::SerializeMaterial(Materials[MaterialIndex], MaterialItem);

						// Add the shared material
						FCPMaterialHelper::AddMaterial(Materials[MaterialIndex], MaterialItem);

						MaterialRefs.Add(FMPAssetRef(MaterialItem.id, MaterialItem.name, FPAssetType::Material, FName(Materials[MaterialIndex]->GetPathName()), NAME_None));
					}
				}

				// Get the HISM transform instances
				TArray<FTransform> Transforms;
				FPMeshHelper::GetHISMInstances(HISMComponent, Transforms);

				FMPMeshComponent SMComponent;

				UStaticMesh* StaticMesh = HISMComponent->GetStaticMesh();
				FGuid MeshID = StaticMesh->GetLightingGuid();
				FMPAssetRef MeshRef = FMPAssetRef(MeshID, FName(StaticMesh->GetName()), FPAssetType::Mesh, FName(StaticMesh->GetPathName()), NAME_None);

				// Does the collision primitive(s) exist for the mesh
				if (!FCPCollisionHelper::IsCollisionExist(MeshID))
				{
					if (FMPCollision CollisionItem; FCPCollisionHelper::SerializeCollisionItem(StaticMesh, CollisionItem))
					{
						CollisionItem.MeshID = MeshID;
						FCPCollisionHelper::AddCollisionItem(MeshID, CollisionItem);
					}
				}

				// Create the actor items
				TArray<FMPActor> ActorItems;
				FMPActor ActorItem;
				for (FTransform Transform : Transforms)
				{
					ActorItem.id = FGuid::NewGuid();
					ActorItem.pid = ActorItem.id;
					ActorItem.name = FName(InActor->GetActorLabel());
					ActorItem.type = FPTargetType::StaticMeshActor;
					ActorItem.component.mesh_comp.mesh_ref = MeshRef;
					ActorItem.component.mesh_comp.material_refs = MaterialRefs;
					ActorItem.transform = Transform;
					ActorItems.Add(ActorItem);
					
					FCPStatsHelper::UpdateMeshGroups(MeshRef, MaterialRefs);
				}

				Package.actors.Append(ActorItems);

				FCPStatsHelper::TotalActors += ActorItems.Num();
				
				if (ShowLogs)
					UE_LOG(LogTemp, Log, TEXT("VWCP - [ HISM Root Actor: %s, Actor Items: %d ]"), *InActor->GetActorLabel(), ActorItems.Num());
			}
			else
			// Other
			{
				TArray<FMPActor> ActorItems;

				SerializeActorHierarchy(InActor, ActorItems, Package.materials);

				if (ActorItems.IsEmpty())
				{
					UE_LOG(LogTemp, Log, TEXT("VWCP - [ Empty Root Actor: %s ]"), *InActor->GetActorLabel());
				}
				else
				{
					Package.actors.Append(ActorItems);

					FCPStatsHelper::TotalActors += ActorItems.Num();

					if (ShowLogs)
						UE_LOG(LogTemp, Log, TEXT("VWCP - [ Root Actor Hierarchy: %s,  Actor Items: %d ]"), *InActor->GetActorLabel(), ActorItems.Num());
				}
			}
	}
}

/**
 * Serializes the Blueprint Actor’s scene‑component hierarchy starting from its RootComponent.
 *
 * Only components parented under the RootComponent are traversed. The root itself
 * is never exported, but its entire scene‑component subtree is preserved including
 * any other scene components.
 *
 * Components that fail ShouldInclude() are skipped, and their children inherit
 * their nearest exported ancestor. If a component has no exported parent, it
 * becomes a root in the exported hierarchy (pid == id).
 *
 * The result is a flat list of exported scene components with stable ids, pids,
 * transforms, and type information, reflecting the filtered component hierarchy.
 */
void UVW_Creators_PluginBPLibrary::FlattenBlueprintActor(
    const AActor* InRootActor,
    TArray<FMPActor>& OutActorItems)
{
    if (!InRootActor)
        return;

    USceneComponent* RootComp = InRootActor->GetRootComponent();
    if (!RootComp)
        return;

    int32 CurrentId = 0;
    TMap<UObject*, int32> ObjectToId;
    TSet<UObject*> ExportedObjects; // <- NEW

    auto ShouldInclude = [](const UObject* Obj) -> bool
    {
        // Example filter: skip billboard components
        return !Obj->IsA<UBillboardComponent>();
    };

    TFunction<int32(UObject*, UObject*)> Traverse = [&](UObject* Obj, const UObject* Parent) -> int32
    {
        if (!Obj)
            return -1;

        bool bIsRoot = (Parent == nullptr);

        int32 ParentId = (Parent && ObjectToId.Contains(Parent))
            ? ObjectToId[Parent]
            : -1;

        int32 MyId = -1;

        if (ShouldInclude(Obj))
        {
            // Assign an ID to every included object (including root),
            // so children can reference it by ID if needed.
            MyId = CurrentId++;
            ObjectToId.Add(Obj, MyId);

            // Skip serializing the root component itself
            if (!bIsRoot)
            {
                USceneComponent* Comp = Cast<USceneComponent>(Obj);
                if (!Comp)
                    return MyId;

                FMPActor ActorItem;
                ActorItem.name = FName(Obj->GetName());
                ActorItem.id = GetActorID(MyId);

                // Parent is only valid if it was actually exported
                bool bParentExported =
                    Parent && ExportedObjects.Contains(Parent);

                if (bParentExported)
                {
                    ActorItem.pid = GetActorID(ObjectToId[Parent]);
                }
                else
                {
                    // No parent in the EXPORTED hierarchy => this is a root in export
                    ActorItem.pid = ActorItem.id;
                }

                ActorItem.transform = Comp->GetComponentTransform();

                if (Comp->GetClass() == UStaticMeshComponent::StaticClass())
                {
                    FPMeshHelper::SerializeMeshComponent(
                        Cast<UStaticMeshComponent>(Comp),
                        ActorItem,
                        Package.materials);

                    ActorItem.type = FPTargetType::StaticMeshActor;
                }
                else if (Comp->GetClass() == UDecalComponent::StaticClass())
                {
                	FCPMaterialHelper::SerializeDecalComponent(
						Cast<UDecalComponent>(Comp),
						ActorItem,
						Package.materials);

                	ActorItem.type = FPTargetType::DecalActor;
                }
                else if (Comp->GetClass() == USpotLightComponent::StaticClass())
                {
                    FCPLightHelper::SerializeSpotLightComponent(
                        Cast<USpotLightComponent>(Comp),
                        ActorItem);

                    ActorItem.type = FPTargetType::SpotLightActor;
                }
                else if (Comp->GetClass() == UPointLightComponent::StaticClass())
                {
                    FCPLightHelper::SerializePointLightComponent(
                        Cast<UPointLightComponent>(Comp),
                        ActorItem);

                    ActorItem.type = FPTargetType::PointLightActor;
                }
                else
                {
                    ActorItem.type = FPTargetType::SceneActor;
                }

                OutActorItems.Add(ActorItem);
                ExportedObjects.Add(Obj); // <- mark as actually exported
            }
        }
        else
        {
            // If excluded, just propagate the parent's ID
            MyId = ParentId;
        }

        // Parent for children is still the logical hierarchy parent,
        // not the "exported" parent; export status is decided per-node.
        UObject* EffectiveParent = Obj;

        if (USceneComponent* SceneComp = Cast<USceneComponent>(Obj))
        {
            for (USceneComponent* Child : SceneComp->GetAttachChildren())
            {
                Traverse(Child, EffectiveParent);
            }
        }

        return MyId;
    };

    // Start traversal at the root component (but do not serialize it)
    Traverse(RootComp, nullptr);
}


/**
 * Serialize the actor hierarchy recursively in winding order
 * @param InActor - Serialize Actors
 * @param OutActors - Array of actors in hierarchy winding order
 * @param OutMaterialItems - Array of material item
 */
void UVW_Creators_PluginBPLibrary::SerializeActorHierarchy(const AActor* InActor, TArray<FMPActor>& OutActors, TArray<FMPMaterial>& OutMaterialItems)
{
	if (!InActor) return;

	// Serialize actor
	FMPActor ActorItem;
	if (SerializeActorComponent(InActor, ActorItem, OutMaterialItems))
	{
		OutActors.Add(ActorItem);
	}

	// Get children
	TArray<AActor*> ChildActors;
	InActor->GetAttachedActors(ChildActors);

	for (AActor* Child : ChildActors)
		SerializeActorHierarchy(Child, OutActors, Package.materials);
}

/**
 * Serialize the actor
 * @param InActor - Actor
 * @param OutActorItem - Actor item
 * @param OutMaterialItems - Array of material item
 * @return Success - true, Fail - false
 */
bool UVW_Creators_PluginBPLibrary::SerializeActorComponent(const AActor* InActor, FMPActor& OutActorItem, TArray<FMPMaterial>& OutMaterialItems)
{
	bool bSuccess = false;

	if (UStaticMeshComponent* SMComponent = InActor->FindComponentByClass<UStaticMeshComponent>())
	{
		if (FPMeshHelper::SerializeMeshComponent(SMComponent, OutActorItem, OutMaterialItems))
		{
			OutActorItem.type = FPTargetType::StaticMeshActor;
			bSuccess = true;
		}
	}
	else if (UDecalComponent* DecalComponent = InActor->FindComponentByClass<UDecalComponent>())
	{
		if (FCPMaterialHelper::SerializeDecalComponent(DecalComponent, OutActorItem, OutMaterialItems))
		{
			OutActorItem.type = FPTargetType::DecalActor;
			bSuccess = true;
		}
	}
	else if (USpotLightComponent* SLComponent = InActor->FindComponentByClass<USpotLightComponent>())
	{
		if (FCPLightHelper::SerializeSpotLightComponent(SLComponent, OutActorItem))
		{
			OutActorItem.type = FPTargetType::SpotLightActor;
			bSuccess = true;
		}
	}
	else if (UPointLightComponent* PLComponent = InActor->FindComponentByClass<UPointLightComponent>())
	{
		if (FCPLightHelper::SerializePointLightComponent(PLComponent, OutActorItem))
		{
			OutActorItem.type = FPTargetType::PointLightActor;
			bSuccess = true;
		}
	}
	else
	{
		FCPStatsHelper::TotalSceneActors++;
		OutActorItem.type = FPTargetType::SceneActor;
		bSuccess = true;
	}

	if (bSuccess)
	{
		AActor* ParentActor = InActor->GetAttachParentActor();
		OutActorItem.id = GetActorID(InActor->GetUniqueID());
		OutActorItem.pid = ParentActor ? GetActorID(ParentActor->GetUniqueID()) : GetActorID(InActor->GetUniqueID());
		OutActorItem.name = FName(InActor->GetActorLabel());
		OutActorItem.transform = InActor->GetRootComponent()->GetComponentTransform();
	}

	return bSuccess;
}

// Serialize the asset
void UVW_Creators_PluginBPLibrary::SerializeAsset(const FMPAssetRef& InAssetRef)
{
	if (FPCacheHelper::AssetExists(InAssetRef.id, InAssetRef.type))
	{
		UE_LOG(LogTemp, Log, TEXT("VWCP - [ Asset Cached, Asset ID: %s, Asset Name: %s, Type: %s ]"), 
			*InAssetRef.id.ToString(), *InAssetRef.name.ToString(), *InAssetRef.type.ToString());
		return;
	}

	if (InAssetRef.type == FPAssetType::Mesh)
		FCPPAKUtility::CreatePAK(InAssetRef.id, FPAssetType::Mesh, InAssetRef.name, InAssetRef.path);
	else
	if (InAssetRef.type == FPAssetType::Texture)
		FCPMaterialHelper::SerializeTexture(InAssetRef.id, InAssetRef.name, InAssetRef.path);
}

/**
 * Save creator plugin config data
 * @param InShowLogs - Show output logs
 * @param InShowJson - Show json
 * @param InClearAllAssets - Clear all assets (includes mesh and texture assets)
 * @param InDontPackageAssets  - Show crunch output logs
 * @param InPackageName - Package name
 * @param InPackageDescription - Package description
 * @param InPackageAuthor - Package author
 * @param InPackagePath - Package path
 */
void UVW_Creators_PluginBPLibrary::SaveConfig(
	const bool InClearLogs,
	const bool InShowLogs,
	const bool InShowJson,
	const bool InClearAllAssets,
	const bool InDontPackageAssets,
	const FString& InPackageName,
	const FString& InPackageDescription,
	const FString& InPackageAuthor,
	const FString& InPackagePath)
{
	ClearLogs = InClearLogs;
	ShowLogs = InShowLogs;
	ShowJson = InShowJson;
	ClearAllAssets = InClearAllAssets;
	DontPackageAssets = InDontPackageAssets;
	PackageName = InPackageName;
	PackageDescription = InPackageDescription;
	PackageAuthor = InPackageAuthor;
	PackagePath = InPackagePath;

	FVWPluginConfig PackageConfig(ClearLogs, ShowLogs, ShowJson, ClearAllAssets, DontPackageAssets, PackageName, PackageDescription, PackageAuthor, PackagePath);

	FString OutputString;

	FJsonObjectConverter::UStructToJsonObjectString(PackageConfig, OutputString, 0, 0, 0, nullptr, true);

	FString FilePath = FPaths::ProjectSavedDir() + PluginINI;

	FFileHelper::SaveStringToFile(OutputString, *FilePath);
}

/**
 * Load plugin config data
 * @param PluginConfig - Plugin config structure
 * @return true - success, false, failed
 */
bool UVW_Creators_PluginBPLibrary::LoadConfig(FVWPluginConfig& PluginConfig)
{
	const FString FilePath = FPaths::ProjectSavedDir() + PluginINI;

	if (FString JSON; FFileHelper::LoadFileToString(JSON, *FilePath))
	{
		if (!FJsonObjectConverter::JsonObjectStringToUStruct(JSON, &PluginConfig, 0, 0))
		{
			UE_LOG(LogTemp, Error, TEXT("VW Creators Plugin - [ Status : VW plugin config load error ]"));
			return false;
		}

		ClearLogs = PluginConfig.ClearLogs;
		ShowLogs = PluginConfig.ShowLogs;
		ShowJson = PluginConfig.ShowJson;
		ClearAllAssets = PluginConfig.ClearAllAssets;
		DontPackageAssets = PluginConfig.DontPackageAssets;
		PackageName = PluginConfig.PackageName;
		PackageDescription = PluginConfig.PackageDescription;
		PackageAuthor = PluginConfig.PackageAuthor;
		PackagePath = PluginConfig.PackagePath;
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("VW Creators Plugin - [ Status: Creating load package editor utility config data : %s]"), *FilePath);

		SaveConfig(
			ClearLogs,
			ShowLogs,
			ShowJson,
			ClearAllAssets,
			DontPackageAssets,
			PackageName,
			PackageDescription,
			PackageAuthor,
			PackagePath);
	}

	return true;
}

/**
 * Get actor identifier (guid)
 * @param InID - Actor internal ID
 * @return Actor Identifier (guid)
 */
FGuid UVW_Creators_PluginBPLibrary::GetActorID(const int32 InID)
{
	if (ActorIDMap.Contains(InID))
		return ActorIDMap[InID];

	FGuid Guid = FGuid::NewGuid();
	ActorIDMap.Add(InID, Guid);
	return Guid;
}
