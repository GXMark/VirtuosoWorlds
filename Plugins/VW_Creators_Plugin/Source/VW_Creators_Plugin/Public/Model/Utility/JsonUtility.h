#pragma once
#include "Model/Package/MPPackage.h"

class FJsonUtility
{
public:
	static FString ToJsonString(const FMPPackage& Package);
};
