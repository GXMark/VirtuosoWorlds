#pragma once

#include "CoreMinimal.h"
#include "Model/Network/VMRepSpatialItemNet.h"
#include "Templates/Function.h"
#include "VRegionRenderQueue.generated.h"

class UMaterialInstanceDynamic;

UENUM()
enum class EVRegionRenderWorkType : uint8
{
	SetMesh,
	ApplyMaterials,
	ApplyTextureParams,
	Destroy,
	SetTransform
};

USTRUCT()
struct VWCLIENT_API FVRegionRenderWorkItem
{
	GENERATED_BODY()

	EVRegionRenderWorkType WorkType = EVRegionRenderWorkType::SetMesh;
	FGuid ItemId;
	FMeshAssetId MeshAssetId;
	TArray<uint32> MaterialIdsBySlot;
	TMap<FName, float> ScalarTextureParameters;
	TMap<FName, FLinearColor> VectorTextureParameters;
	FGuid MaterialId;
	FGuid TextureId;
	FName TextureParameter;
	FGuid SourceItemId;
	uint32 SourceGeneration = 0;
	TWeakObjectPtr<UMaterialInstanceDynamic> MaterialInstance;
	FTransform Transform;
	int32 Weight = 1;

	void RefreshWeight();

	static int32 CalculateWeight(EVRegionRenderWorkType Type);
};

class VWCLIENT_API FVRegionRenderQueue
{
public:
	void Enqueue(FVRegionRenderWorkItem&& Item);
	void Drain(
		int32 BudgetPoints,
		int32& OutAppliedCount,
		int32& OutBudgetRemaining,
		const TFunctionRef<bool(const FVRegionRenderWorkItem&)>& ApplyWork);
	int32 Num() const;

private:
	struct FVRegionRenderWorkKey
	{
		FGuid ItemId;
		EVRegionRenderWorkType WorkType = EVRegionRenderWorkType::SetMesh;

		friend bool operator==(const FVRegionRenderWorkKey& Left, const FVRegionRenderWorkKey& Right)
		{
			return Left.ItemId == Right.ItemId && Left.WorkType == Right.WorkType;
		}
	};

	friend uint32 GetTypeHash(const FVRegionRenderWorkKey& Key)
	{
		return HashCombine(GetTypeHash(Key.ItemId), GetTypeHash(static_cast<uint8>(Key.WorkType)));
	}

	bool ShouldCoalesce(EVRegionRenderWorkType WorkType) const;

	TArray<FVRegionRenderWorkItem> Queue;
	TMap<FVRegionRenderWorkKey, int32> CoalescedIndices;
};
