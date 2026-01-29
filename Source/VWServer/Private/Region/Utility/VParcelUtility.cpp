#include "Region/Utility/VParcelUtility.h"
#include "Model/Land/VParcel.h"
#include "Model/Land/VRegion.h"


bool FVParcelUtility::Validate(const FVRegion& InRegion, const int32 InGridSize)
{
	if (!ValidateNames(InRegion))
	{
		UE_LOG(LogTemp, Error, TEXT("Parcel Utility - Validate [ Status: Validate Names Failed ]"));
		return false;
	}

	if (!ValidateParcels(InRegion, InGridSize))
	{
		UE_LOG(LogTemp, Error, TEXT("Parcel Utility - Validate [ Status: Validate Parcels Failed ]"));
		return false;
	}

	return true;
}

bool FVParcelUtility::ValidateNames(const FVRegion& InRegion)
{
	bool Valid = true;

	if (InRegion.name.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("Parcel Utility - ValidateNames [ Status: Region name is not defined ]"));
		Valid = false;
	}

	if (!InRegion.id.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Parcel Utility - ValidateNames [ Status: Region ID is not a valid format ]"));
		Valid = false;
	}

	if (InRegion.owner_id.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("Parcel Utility - ValidateNames [ Status: Region owner ID is not defined ]"));
		Valid = false;
	}

	for (FVParcel Parcel : InRegion.parcels)
	{
		if (Parcel.name.IsEmpty())
		{
			UE_LOG(LogTemp, Error, TEXT("Parcel Utility - ValidateNames [ Status: Parcel name is not defined ]"));
			Valid = false;
		}

		if (!Parcel.id.IsValid())
		{
			UE_LOG(LogTemp, Error, TEXT("Parcel Utility - ValidateNames [ Status: Parcel ID is not a valid format ]"));
			Valid = false;
		}
	}

	return Valid;
}

bool FVParcelUtility::ValidateParcels(const FVRegion& InRegion, const int32 InGridSize)
{
	const TArray<FVParcel>& Parcels = InRegion.parcels;

	// 1. Check divisibility and accumulate total volume
	int64 TotalParcelVolume = 0;
	for (const FVParcel& P : Parcels) {
		if (P.size.X % InGridSize != 0 || P.size.Y % InGridSize != 0 || P.size.Z % InGridSize != 0) {
			UE_LOG(LogTemp, Error, TEXT("Parcel at (%d,%d,%d) has invalid size."), P.position.X, P.position.Y, P.position.Z);
			return false;
		}
		TotalParcelVolume += static_cast<int64>(P.size.X) * P.size.Y * P.size.Z;
	}

	// 2. Check for overlaps
	for (int32 i = 0; i < Parcels.Num(); ++i) {
		const FVParcel& A = Parcels[i];
		for (int32 j = i + 1; j < Parcels.Num(); ++j) {
			const FVParcel& B = Parcels[j];

			bool OverlapX = A.position.X < B.position.X + B.size.X && A.position.X + A.size.X > B.position.X;
			bool OverlapY = A.position.Y < B.position.Y + B.size.Y && A.position.Y + A.size.Y > B.position.Y;
			bool OverlapZ = A.position.Z < B.position.Z + B.size.Z && A.position.Z + A.size.Z > B.position.Z;

			if (OverlapX && OverlapY && OverlapZ) {
				UE_LOG(LogTemp, Error, TEXT("Overlap detected between parcels at (%d,%d,%d) and (%d,%d,%d)."),
					A.position.X, A.position.Y, A.position.Z, B.position.X, B.position.Y, B.position.Z);
				return false;
			}
		}
	}

	// 3. Check total volume matches region
	int64 RegionVolume = static_cast<int64>(InRegion.size.X) * InRegion.size.Y * InRegion.size.Z;
	if (TotalParcelVolume != RegionVolume) {
		UE_LOG(LogTemp, Error, TEXT("Total parcel volume (%lld) does not match region volume (%lld)."),
			TotalParcelVolume, RegionVolume);
		return false;
	}

	//UE_LOG(LogTemp, Display, TEXT("All parcels are valid, non-overlapping, and fully cover the region."));
	return true;
}


bool FVParcelUtility::ValidateParcelSizes(const FVRegion& InRegion, const int32 InGridSize)
{
	const TArray<FVParcel>& Parcels = InRegion.parcels;

	for (const FVParcel& Parcel : Parcels)
	{
		if (!IsDivisibleByGridSize(Parcel, InGridSize))
		{
			UE_LOG(LogTemp, Error, TEXT("Parcel Utility - ValidateParcels [ Parcel Name: %s, Parcel at (%d,%d,%d) has invalid size. ]"), *Parcel.name, Parcel.position.X, Parcel.position.Y, Parcel.position.Z);
			return false;
		}
		if (!IsAlignedToGrid(Parcel, InGridSize))
		{
			UE_LOG(LogTemp, Error, TEXT("Parcel Utility - ValidateParcels [ Parcel Name: %s, Parcel at (%d,%d,%d) is not aligned to 32x32x32 grid. ]"), *Parcel.name, Parcel.position.X, Parcel.position.Y, Parcel.position.Z);
			return false;
		}
		if (!IsWithinRegion(Parcel, InRegion))
		{
			UE_LOG(LogTemp, Error, TEXT("Parcel Utility - ValidateParcels [ Parcel Name: %s, Parcel at (%d,%d,%d) exceeds region boundaries. ]"), *Parcel.name, Parcel.position.X, Parcel.position.Y, Parcel.position.Z);
			return false;
		}
	}

	for (int32 i = 0; i < Parcels.Num(); ++i)
	{
		for (int32 j = i + 1; j < Parcels.Num(); ++j)
		{
			if (!AreAdjacentAndAligned(Parcels[i], Parcels[j]))
			{
				UE_LOG(LogTemp, Error, TEXT("Parcel Utility - ValidateParcels [ Parcels (%s):(%d,%d,%d) and (%s):(%d,%d,%d) are not perfectly aligned or adjacent. ]"),
					   *Parcels[i].name,
					   Parcels[i].position.X, Parcels[i].position.Y, Parcels[i].position.Z,
					   *Parcels[j].name,
				       Parcels[j].position.X, Parcels[j].position.Y, Parcels[j].position.Z);
				return false;
			}
		}
	}

	return true;
}

bool FVParcelUtility::ValidateFullParcelCoverage(const FVRegion& InRegion, const int32 InGridSize)
{
	const TArray<FVParcel>& Parcels = InRegion.parcels;

	// Convert region size to grid dimensions
	const int32 GridX = InRegion.size.X / InGridSize;
	const int32 GridY = InRegion.size.Y / InGridSize;
	const int32 GridZ = InRegion.size.Z / InGridSize;

	// Create a 3D occupancy grid
	TArray<bool> Occupied;
	Occupied.Init(false, GridX * GridY * GridZ);

	auto GetIndex = [&](int32 x, int32 y, int32 z)
	{
		return x + GridX * (y + GridY * z);
	};

	// Mark occupied cells
	for (const FVParcel& Parcel : Parcels)
	{
		if (Parcel.size.X % InGridSize != 0 || Parcel.size.Y % InGridSize != 0 || Parcel.size.Z % InGridSize != 0 ||
			Parcel.position.X % InGridSize != 0 || Parcel.position.Y % InGridSize != 0 || Parcel.position.Z % InGridSize != 0)
		{
			UE_LOG(LogTemp, Error, TEXT("Parcel Utility - ValidateFullCoverage [ Parcel (%s) at (%d,%d,%d) has invalid alignment or size. ]"),
				*Parcel.name, Parcel.position.X, Parcel.position.Y, Parcel.position.Z);
			return false;
		}

		int32 StartX = Parcel.position.X / InGridSize;
		int32 StartY = Parcel.position.Y / InGridSize;
		int32 StartZ = Parcel.position.Z / InGridSize;

		int32 EndX = StartX + Parcel.size.X / InGridSize;
		int32 EndY = StartY + Parcel.size.Y / InGridSize;
		int32 EndZ = StartZ + Parcel.size.Z / InGridSize;

		for (int32 x = StartX; x < EndX; ++x)
		{
			for (int32 y = StartY; y < EndY; ++y)
			{
				for (int32 z = StartZ; z < EndZ; ++z)
				{
					int32 Index = GetIndex(x, y, z);
					if (Index >= Occupied.Num())
					{
						UE_LOG(LogTemp, Error, TEXT("Parcel Utility - ValidateFullCoverage [ Parcel (%s) at (%d,%d,%d) exceeds region bounds. ]"),
							*Parcel.name, Parcel.position.X, Parcel.position.Y, Parcel.position.Z);
						return false;
					}
					if (Occupied[Index])
					{
						UE_LOG(LogTemp, Error, TEXT("Parcel Utility - ValidateFullCoverage [ Parcel (%s) : Overlap detected at grid cell (%d,%d,%d). ]"), *Parcel.name, x, y, z);
						return false;
					}
					Occupied[Index] = true;
				}
			}
		}
	}

	// Check for uncovered cells
	for (int32 i = 0; i < Occupied.Num(); ++i)
	{
		if (!Occupied[i])
		{
			int32 x = i % GridX;
			int32 y = (i / GridX) % GridY;
			int32 z = i / (GridX * GridY);
			UE_LOG(LogTemp, Error, TEXT("Parcel Utility - ValidateFullCoverage [ Uncovered cell at (%d,%d,%d). ]"), x * InGridSize, y * InGridSize, z * InGridSize);
			return false;
		}
	}

	return true;
}

bool FVParcelUtility::IsDivisibleByGridSize(const FVParcel& InParcel, const int32 InGridSize)
{
	return InParcel.size.X % InGridSize == 0 &&
		InParcel.size.Y % InGridSize == 0 &&
		InParcel.size.Z % InGridSize == 0;
}

bool FVParcelUtility::IsAlignedToGrid(const FVParcel& InParcel, const int32 InGridSize)
{
	return InParcel.position.X % InGridSize == 0 &&
		InParcel.position.Y % InGridSize == 0 &&
		InParcel.position.Z % InGridSize == 0;
}

bool FVParcelUtility::IsWithinRegion(const FVParcel& InParcel, const FVRegion& InRegion)
{
	return InParcel.position.X >= 0 && InParcel.position.Y >= 0 && InParcel.position.Z >= 0 &&
		InParcel.position.X + InParcel.size.X <= InRegion.size.X &&
		InParcel.position.Y + InParcel.size.Y <= InRegion.size.Y &&
		InParcel.position.Z + InParcel.size.Z <= InRegion.size.Z;
}

bool FVParcelUtility::AreAdjacentAndAligned(const FVParcel& InA, const FVParcel& InB)
{
	bool XAligned = InA.position.Y == InB.position.Y && InA.size.Y == InB.size.Y &&
		InA.position.Z == InB.position.Z && InA.size.Z == InB.size.Z &&
		(InA.position.X + InA.size.X == InB.position.X || InB.position.X + InB.size.X == InA.position.X);

	bool YAligned = InA.position.X == InB.position.X && InA.size.X == InB.size.X &&
		InA.position.Z == InB.position.Z && InA.size.Z == InB.size.Z &&
		(InA.position.Y + InA.size.Y == InB.position.Y || InB.position.Y + InB.size.Y == InA.position.Y);

	bool ZAligned = InA.position.X == InB.position.X && InA.size.X == InB.size.X &&
		InA.position.Y == InB.position.Y && InA.size.Y == InB.size.Y &&
		(InA.position.Z + InA.size.Z == InB.position.Z || InB.position.Z + InB.size.Z == InA.position.Z);

	return XAligned || YAligned || ZAligned;
}

/**
* Is the position inside the box bounds?
* @param InBounds - Box bounds
* @param InPosition - World position
* @return true - Position is inside the bounds, false - Position is outside the bounds
*/
bool FVParcelUtility::IsInsideBounds(const FBox& InBounds, const FVector3f& InPosition)
{
	return InBounds.IsInside(FVector(InPosition));
}

/**
 * Create a box given its position and size
 * Position is the world location of the box's minimum corner (left-bottom-back)
 * @param InPosition - World position representing a min left corner of the box
 * @param InSize -  width (X), depth (Y), height (Z)
 * @return Box
 */
FBox FVParcelUtility::CreateBoxFromMinAndSize(const FIntVector& InPosition, const FIntVector& InSize)
{
	FVector Min(InPosition.X, InPosition.Y, InPosition.Z);        // convert FVector3f → FVector
	FVector Max = Min + FVector(InSize.X, InSize.Y, InSize.Z);    // add size to min to get max
	return FBox(Min, Max);
}
