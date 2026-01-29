#pragma once
#include "CoreMinimal.h"
#include "Containers/Queue.h"
#include "Model/Network/VMMeshComponentNet.h"
#include "Asset/Agent/VAnimationAgent.h"
#include "Asset/Agent/VAudioAgent.h"
#include "Asset/Agent/VLightAgent.h"
#include "Asset/Agent/VMaterialAgent.h"
#include "Asset/Agent/VTextureAgent.h"
#include "Asset/Agent/VMeshAgent.h"
#include "Asset/Agent/VSkeletalAgent.h"
#include "Subsystems/WorldSubsystem.h"
#include "VAssetManager.generated.h"

class UStaticMesh;
DECLARE_DELEGATE_OneParam(FVOnStaticMeshLoaded, UStaticMesh*);

class UTexture2D;
DECLARE_DELEGATE_OneParam(FVOnTextureLoaded, UTexture2D*);

class UMaterialInstanceDynamic;
DECLARE_DELEGATE_OneParam(FVOnMaterialInstanceLoaded, UMaterialInstanceDynamic*);

/**
 * Asset Manager
 * World-scoped runtime subsystem.
 */
UCLASS()
class VWCLIENT_API UVAssetManager : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& InOutCollection) override;
	virtual void Deinitialize() override;

	static UVAssetManager* Get(const UWorld* InWorld)
	{
		return InWorld ? InWorld->GetSubsystem<UVAssetManager>() : nullptr;
	}
	
	// Main Properties
	static FString CachePath;
	static FString CDN_URL;
	static FString CDN_Password;
	
	// API Cache Management
	bool Initialize(const FString& InCachePath, bool InEmptyCache, const FString& InAssetServerURL, const FString& InAssetServerPassword, bool InServerMode) const;
	bool IsDiskCached(const FGuid& InAssetID, const FName& InAssetType) const;
	bool IsMemoryCached(const FGuid& InAssetID, const FName& InAssetType) const;
	void EmptyCache() const;
	
	void UpdateMaterialItem(const FVMMaterial& InMaterialItem) const;

	// Async Mesh Resolve (Editor: mesh_path, Packaged: Mount -> LoadPAKAsync -> Unmount)
	void RequestStaticMeshAsync(const FVMMeshComponentNet& InMesh, FVOnStaticMeshLoaded OutCallback);

	// Async Texture Resolve (Editor: texture.asset_ref.path -> editor asset registry, Packaged: cache/remote -> CRN decode)
	void RequestTextureAsync(const FVMTexture& InTexture, FVOnTextureLoaded OutCallback);

	// Async Material Resolve (Creates a MID, then resolves dependent textures via RequestTextureAsync)
	void RequestMaterialInstanceAsync(const FGuid& InMaterialId, FVOnMaterialInstanceLoaded OutCallback);

	// Called by the region stream when material items arrive. Adds items, creates MIDs, and kicks texture resolves.
	void SubmitMaterialItems(const TArray<FVMMaterial>& InMaterials);

	/**
	 * Throttle for concurrent remote asset requests (download + load).
	 * Applies to packaged runtime only.
	 */
	UPROPERTY(EditAnywhere, Category = "Virtuoso|Assets")
	int32 MaxConcurrentAssetRequests = 4;

	/**
	 * Retry policy for remote asset acquisition (download and/or subsequent load).
	 */
	UPROPERTY(EditAnywhere, Category = "Virtuoso|Assets")
	int32 MaxAssetAttempts = 3;
	
	// References
	UPROPERTY()
	TObjectPtr<UVMaterialAgent> MaterialAgent;
	
	UPROPERTY()
	TObjectPtr<UVTextureAgent> TextureAgent;
		
	UPROPERTY()
	TObjectPtr<UVMeshAgent> MeshAgent;
	
	UPROPERTY()
	TObjectPtr<UVSkeletalAgent> SkeletalAgent;
	
	UPROPERTY()
	TObjectPtr<UVLightAgent> LightAgent;
	
	UPROPERTY()
	TObjectPtr<UVAnimationAgent> AnimationAgent;
	
	UPROPERTY()
	TObjectPtr<UVAudioAgent> AudioAgent;

private:
	// Coalesce concurrent requests per MeshId
	TMap<FGuid, TArray<FVOnStaticMeshLoaded>> PendingStaticMeshLoads;

	// Coalesce concurrent requests per TextureId
	TMap<FGuid, TArray<FVOnTextureLoaded>> PendingTextureLoads;

	// Coalesce concurrent requests per MaterialId
	TMap<FGuid, TArray<FVOnMaterialInstanceLoaded>> PendingMaterialLoads;

	// --------------------
	// Remote request queue (packaged runtime)
	// --------------------
	struct FVAssetKey
	{
		FGuid Id;
		FName Type;

		FVAssetKey() : Id(), Type(NAME_None) {}
		FVAssetKey(const FGuid& InId, const FName& InType) : Id(InId), Type(InType) {}

		friend bool operator==(const FVAssetKey& A, const FVAssetKey& B)
		{
			return A.Id == B.Id && A.Type == B.Type;
		}
	};

	friend uint32 GetTypeHash(const FVAssetKey& Key)
	{
		return HashCombine(GetTypeHash(Key.Id), GetTypeHash(Key.Type));
	}

	struct FVRemoteAssetState
	{
		FName Name = NAME_None; // required for PAK load
		bool bLinear = false;  // textures only
		int32 AttemptsMade = 0;
		int32 MaxAttempts = 3;
		bool bInProgress = false;
	};

	// Pending FIFO and state tracking
	TQueue<FVAssetKey> PendingAssetQueue;
	TSet<FVAssetKey> QueuedAssetKeys;
	TMap<FVAssetKey, FVRemoteAssetState> RemoteAssetStates;
	int32 ActiveAssetRequests = 0;

	void EnqueueRemoteAssetRequest(const FVAssetKey& Key, const FName& InName, bool bInLinear = false);
	void PumpRemoteAssetQueue();
	void ProcessRemoteAssetKey(const FVAssetKey& Key);
	void FinishRemoteAssetKey(const FVAssetKey& Key);
	FString MakeObjectKey(const FVAssetKey& Key) const;
	FString MakeLocalCachePath(const FString& ObjectKey) const;

	void CompleteStaticMeshLoad(const FGuid& InMeshId, UStaticMesh* InOutMesh);
	void CompleteTextureLoad(const FGuid& InTextureId, UTexture2D* InOutTexture);
	void CompleteMaterialLoad(const FGuid& InMaterialId, UMaterialInstanceDynamic* InOutMaterial);

	void TryResolveMaterialInstance(const FGuid& InMaterialId);

	bool ShouldUseEditorAssetPathResolution() const;
	static FSoftObjectPath MakeSoftObjectPathFromAssetRef(const FName& InPath, const FName& InName);
	
	static bool SetCachePath(const FString& InPath);
};