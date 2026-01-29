#pragma once

#include "CoreMinimal.h"
#include "VTextureAgent.generated.h"

UCLASS()
class VWCLIENT_API UVTextureAgent : public UObject
{
	GENERATED_BODY()
public:
	bool Initialize();

	bool IsLocallyCached(const FGuid& InTextureID, FName InCacheType) const;
	void SetTexture(const FGuid& InTextureID, UTexture2D* InTexture);
	UTexture2D* GetTexture(const FGuid& InKey);

	UPROPERTY()
	TMap<FGuid, UTexture2D*> TextureMap;
	
protected:
	virtual void BeginDestroy() override;
};
