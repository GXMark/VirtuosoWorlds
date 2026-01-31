#include "Region/VRegionRenderQueue.h"

void FVRegionRenderWorkItem::RefreshWeight()
{
	Weight = CalculateWeight(WorkType);
}

int32 FVRegionRenderWorkItem::CalculateWeight(EVRegionRenderWorkType Type)
{
	switch (Type)
	{
	case EVRegionRenderWorkType::SetMesh:
		return 5;
	case EVRegionRenderWorkType::ApplyMaterials:
		return 4;
	case EVRegionRenderWorkType::ApplyTextureParams:
		return 3;
	case EVRegionRenderWorkType::Destroy:
		return 2;
	case EVRegionRenderWorkType::SetTransform:
		return 1;
	default:
		return 1;
	}
}

void FVRegionRenderQueue::Enqueue(FVRegionRenderWorkItem&& Item)
{
	Item.RefreshWeight();

	if (ShouldCoalesce(Item.WorkType))
	{
		const FVRegionRenderWorkKey Key{Item.ItemId, Item.WorkType};
		if (int32* ExistingIndex = CoalescedIndices.Find(Key))
		{
			Queue[*ExistingIndex] = MoveTemp(Item);
			return;
		}

		const int32 NewIndex = Queue.Add(MoveTemp(Item));
		CoalescedIndices.Add(Key, NewIndex);
		return;
	}

	Queue.Add(MoveTemp(Item));
}

void FVRegionRenderQueue::Drain(
	int32 BudgetPoints,
	int32& OutAppliedCount,
	int32& OutBudgetRemaining,
	const TFunctionRef<bool(const FVRegionRenderWorkItem&)>& ApplyWork)
{
	OutAppliedCount = 0;
	OutBudgetRemaining = FMath::Max(0, BudgetPoints);

	if (Queue.IsEmpty() || OutBudgetRemaining <= 0)
	{
		return;
	}

	int32 ItemsToRemove = 0;
	for (const FVRegionRenderWorkItem& Item : Queue)
	{
		if (Item.Weight > OutBudgetRemaining)
		{
			break;
		}

		OutBudgetRemaining -= Item.Weight;
		if (ApplyWork(Item))
		{
			++OutAppliedCount;
		}
		++ItemsToRemove;
	}

	if (ItemsToRemove <= 0)
	{
		return;
	}

	Queue.RemoveAt(0, ItemsToRemove, EAllowShrinking::No);
	CoalescedIndices.Reset();
	for (int32 Index = 0; Index < Queue.Num(); ++Index)
	{
		if (ShouldCoalesce(Queue[Index].WorkType))
		{
			CoalescedIndices.Add({Queue[Index].ItemId, Queue[Index].WorkType}, Index);
		}
	}
}

int32 FVRegionRenderQueue::Num() const
{
	return Queue.Num();
}

bool FVRegionRenderQueue::ShouldCoalesce(EVRegionRenderWorkType WorkType) const
{
	return WorkType == EVRegionRenderWorkType::SetMesh
		|| WorkType == EVRegionRenderWorkType::ApplyMaterials
		|| WorkType == EVRegionRenderWorkType::ApplyTextureParams
		|| WorkType == EVRegionRenderWorkType::SetTransform;
}
