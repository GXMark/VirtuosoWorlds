#pragma once
#include "Model/Land/VRegion.h"

class VWSERVER_API FVParcelUtility
{
public:
	static bool Validate(const FVRegion& InRegion, int32 InGridSize);
private:
	static bool ValidateNames(const FVRegion& InRegion);
	static bool ValidateParcels(const FVRegion& InRegion, int32 InGridSize);
	static bool ValidateParcelSizes(const FVRegion& InRegion, const int32 InGridSize);
	static bool ValidateFullParcelCoverage(const FVRegion& InRegion, const int32 InGridSize);
	static bool IsDivisibleByGridSize(const FVParcel& InParcel, const int32 InGridSize);
	static bool IsAlignedToGrid(const FVParcel& InParcel, const int32 InGridSize);
	static bool IsWithinRegion(const FVParcel& InParcel, const FVRegion& InRegion);
	static bool AreAdjacentAndAligned(const FVParcel& InA, const FVParcel& InB);
	static bool IsInsideBounds(const FBox& InBounds, const FVector3f& InPosition);
	static FBox CreateBoxFromMinAndSize(const FIntVector& InPosition, const FIntVector& InSize);
};
