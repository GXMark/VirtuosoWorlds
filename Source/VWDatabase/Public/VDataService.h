#pragma once

#include "CoreMinimal.h"
#include "Model/Package/VMCollision.h"
#include "VWDatabase/ThirdParty/ObjectBox/include/objectbox.hpp"
#include "Model/Package/VMPackage.h"
#include "Subsystems/WorldSubsystem.h"
#include "VDataService.generated.h"

UCLASS()
class VWDATABASE_API UVDataService : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
	static UVDataService* Get(const UWorld* World)	{ return World ? World->GetSubsystem<UVDataService>() : nullptr; }
	
	// Data Store
	bool InitializeStore();
	//static bool RemapPackageIDs(TArray<FMPackage>& OutPackages);
	bool AddPackages(const TArray<FVMPackage>& InPackages) const;
	bool AddActor(const FString& InKey, const FString& InValue) const;
	bool AddMaterial(const FString& InKey, const FString& InValue) const;
	bool AddCollision(const FString& InKey, const FString& InValue) const;
	void UpdateActor(const FString& InKey, const FString& InValue) const;
	void UpdateMaterial(const FString& InKey, const FString& InValue) const;
	void UpdateCollision(const FString& InKey, const FString& InValue) const;
	bool RemovalAll() const;
	void RemoveActor(const FString& InKey) const;
	void RemoveMaterial(const FString& InKey) const;
	void RemoveCollision(const FString& InKey) const;
	FString GetActor(const FString& InKey) const;
	FString GetMaterial(const FString& InKey) const;
	FString GetCollision(const FString& InKey) const;
	bool GetAllActors(TArray<FVMActor>& OutActorItems) const;
	bool GetAllMaterials(TArray<FVMMaterial>& OutMaterialItems) const;
	bool GetAllCollisions(TArray<FVMCollision>& OutCollisionItems) const;
	uint64_t ActorCount() const;
	uint64_t MaterialCount() const;
	uint64_t CollisionCount() const;

private:
	UPROPERTY()
	TMap<FGuid, FVMActor> ActorItemMap;
	
	UPROPERTY()
	TMap<FGuid, FVMMaterial> MaterialItemMap;
	
	UPROPERTY()
	TMap<FGuid, FVMCollision> CollisionItemMap;
	
	TUniquePtr<obx::Store> Store;
};
