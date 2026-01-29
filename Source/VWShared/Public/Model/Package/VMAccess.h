#pragma once

#include "CoreMinimal.h"
#include "VMAccess.generated.h"

USTRUCT()
struct VWSHARED_API FVMAccess
{
	GENERATED_BODY()

	UPROPERTY()
	bool can_copy;
	UPROPERTY()
	bool can_transfer;
	UPROPERTY()
	bool can_modify;
	UPROPERTY()
	FGuid group_id;

	FVMAccess() : 
		can_copy(false)
		, can_transfer(false)
		, can_modify(false)
		, group_id(FGuid(0, 0, 0, 0))
	{}
	
	FVMAccess(const bool CanCopy, const bool CanTransfer, const bool CanModify, const FGuid& GroupID) :
		can_copy(CanCopy)
		, can_transfer(CanTransfer)
		, can_modify(CanModify)
		, group_id(GroupID)
	{}
};
