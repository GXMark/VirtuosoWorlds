#include "Public/Utility/VCommonUtility.h"

/**
 * Check the guid string format
 * @param InRawGuid - GUID String
 * @return true - valid guid, false - invalid guid
 */
bool FVCommonUtility::CheckGUID(const FString& InRawGuid)
{
	if (InRawGuid.Len() == 32)
	{
		uint32 A = FParse::HexNumber(*InRawGuid.Left(8));
		uint32 B = FParse::HexNumber(*InRawGuid.Mid(8, 8));
		uint32 C = FParse::HexNumber(*InRawGuid.Mid(16, 8));
		uint32 D = FParse::HexNumber(*InRawGuid.Mid(24, 8));

		FGuid ParsedGuid = FGuid(A, B, C, D);

		return ParsedGuid.IsValid();
	}
	return false;
}

/**
 * Remove the negative zero's from the vector
 * @param InValue - Vector
 * @return Value without negative zero
 */
FVector3d FVCommonUtility::RemoveNegativeZero(const FVector3d& InValue)
{
	return FVector3d(FMath::IsNearlyZero(InValue.X) ? 0 : InValue.X,
					 FMath::IsNearlyZero(InValue.Y) ? 0 : InValue.Y,
					 FMath::IsNearlyZero(InValue.Z) ? 0 : InValue.Z);
}