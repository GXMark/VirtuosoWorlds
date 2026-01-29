#pragma once
#include "CoreMinimal.h"
#include "Engine/StaticMesh.h"
#include "VMeshAgent.generated.h"

UCLASS()
class VWCLIENT_API UVMeshAgent : public UObject
{
	GENERATED_BODY()
public:
	bool Initialize();
	
	// Cache
	bool IsLocallyCached(const FGuid& InID, FName InCacheType) const;
	void RemoveAll();
	TArray<FGuid> GetMeshIDs() const;

	// Setter/Getter
	void SetMesh(const FGuid& InMeshID, UStaticMesh* InMesh);
	UStaticMesh* GetMesh(const FGuid& InMeshID);
	
	//bool DeserializeHISMItem(const FMHISMComponent& InHISMItem, UHierarchicalInstancedStaticMeshComponent* OutHISMComponent) const;
	
	static bool HasDistanceFieldData(UStaticMesh* Mesh);
	static bool HasRayTracingGeometry(UStaticMesh* Mesh);

private:
	UPROPERTY()
	TMap<FGuid, UStaticMesh*> SharedMeshMap;
};
