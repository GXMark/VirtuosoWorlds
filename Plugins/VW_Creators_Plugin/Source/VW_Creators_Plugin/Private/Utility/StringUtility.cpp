#include "Utility/StringUtility.h"


/**
 * Remove the negative zero's from the vector
 * @param InValue - Vector
 * @return Value without negative zero
 */
FVector3d FCPStringUtility::RemoveNegativeZero(const FVector3d& InValue)
{
	return FVector3d(FMath::IsNearlyZero(InValue.X) ? 0 : InValue.X,
					 FMath::IsNearlyZero(InValue.Y) ? 0 : InValue.Y,
					 FMath::IsNearlyZero(InValue.Z) ? 0 : InValue.Z);
}

/**
 * Convert uint32 array into a comma seperated string
 * @param InArray - Array of uint32's
 * @return Comma seperated string
 */
FString FCPStringUtility::ConvertUInt32ArrayToString(const TArray<uint32>& InArray)
{
	FString Result;

	for (int32 i = 0; i < InArray.Num(); ++i)
	{
		Result += FString::FromInt(InArray[i]);
		if (i < InArray.Num() - 1)
			Result += TEXT(",");
	}

	return Result;
}

/**
 * Convert comma seperated string to an uint32 array
 * @param InValueStr - Comma seperated string
 * @return Array of uint32's
 */
TArray<uint32> FCPStringUtility::ConvertStringToUInt32Array(const FString& InValueStr)
{
	TArray<FString> StringParts;
	TArray<uint32> UIntArray;

	// Split the string by commas
	InValueStr.ParseIntoArray(StringParts, TEXT(","), true);

	// Convert each string to uint32
	for (const FString& Part : StringParts)
		UIntArray.Add(FCString::Atoi(*Part));

	return UIntArray;
}
