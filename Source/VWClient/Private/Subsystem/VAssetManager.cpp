#include "Subsystem/VAssetManager.h"
#include "IPlatformFilePak.h"
#include "Chaos/AABB.h"
#include "Misc/SecureHash.h"
#include "Misc/FileHelper.h"
#include "Misc/Base64.h"
#include "HAL/PlatformFileManager.h"
#include "Misc/Paths.h"
#include "Model/Constant/VConstants.h"
#include "Utility/VPAKUtility.h"
#include "Utility/VHTTPUtility.h"
#include "Utility/VCrunchUtility.h"
#include "Async/Async.h"
#include "Engine/World.h"

FString UVAssetManager::CachePath;
FString UVAssetManager::CDN_URL;
FString UVAssetManager::CDN_Password;

void UVAssetManager::Initialize(FSubsystemCollectionBase& InOutCollection)
{
	Super::Initialize(InOutCollection);

	MeshAgent = NewObject<UVMeshAgent>(this);
	MaterialAgent = NewObject<UVMaterialAgent>(this);
	TextureAgent = NewObject<UVTextureAgent>(this);
	LightAgent = NewObject<UVLightAgent>(this);
	SkeletalAgent = NewObject<UVSkeletalAgent>(this);
	AudioAgent = NewObject<UVAudioAgent>(this);
	AnimationAgent = NewObject<UVAnimationAgent>(this);

	UE_LOG(LogTemp, Log, TEXT("Cache Manager - Initialize [ Success ]"));
}

void UVAssetManager::Deinitialize()
{
	Super::Deinitialize();

	// Best-effort: clear any queued work so we do not hold callbacks past world teardown.
	{
		FVAssetKey Temp;
		while (PendingAssetQueue.Dequeue(Temp))
		{
		}
	}
	QueuedAssetKeys.Empty();
	RemoteAssetStates.Empty();
	ActiveAssetRequests = 0;
}

FString UVAssetManager::MakeObjectKey(const FVAssetKey& Key) const
{
	if (Key.Type == FVAssetType::Mesh)
	{
		return FString::Printf(TEXT("%s.mesh"), *Key.Id.ToString());
	}
	if (Key.Type == FVAssetType::Texture)
	{
		return FString::Printf(TEXT("%s.texture"), *Key.Id.ToString());
	}
	if (Key.Type == FVAssetType::Package)
	{
		return FString::Printf(TEXT("%s.package"), *Key.Id.ToString());
	}

	// Unknown type
	return FString();
}

FString UVAssetManager::MakeLocalCachePath(const FString& ObjectKey) const
{
	return FPaths::Combine(CachePath, ObjectKey);
}

void UVAssetManager::EnqueueRemoteAssetRequest(const FVAssetKey& Key, const FName& InName, bool bInLinear)
{
	// Only meaningful in packaged runtime; PIE resolves by editor asset registry and does not need remote acquisition.
#if WITH_EDITOR
	if (ShouldUseEditorAssetPathResolution())
	{
		return;
	}
#endif

	if (!Key.Id.IsValid())
	{
		return;
	}

	// Ensure state exists
	FVRemoteAssetState& State = RemoteAssetStates.FindOrAdd(Key);
	if (State.MaxAttempts <= 0)
	{
		State.MaxAttempts = FMath::Max(1, MaxAssetAttempts);
	}
	State.Name = InName;
	State.bLinear = bInLinear;
	State.MaxAttempts = FMath::Max(1, MaxAssetAttempts);

	// If already in progress or already queued, nothing to do.
	if (State.bInProgress || QueuedAssetKeys.Contains(Key))
	{
		return;
	}

	QueuedAssetKeys.Add(Key);
	PendingAssetQueue.Enqueue(Key);
	PumpRemoteAssetQueue();
}

void UVAssetManager::PumpRemoteAssetQueue()
{
	const int32 Cap = FMath::Max(1, MaxConcurrentAssetRequests);

	while (ActiveAssetRequests < Cap)
	{
		FVAssetKey Key;
		if (!PendingAssetQueue.Dequeue(Key))
		{
			break;
		}

		QueuedAssetKeys.Remove(Key);
		ProcessRemoteAssetKey(Key);
	}
}

void UVAssetManager::ProcessRemoteAssetKey(const FVAssetKey& Key)
{
	FVRemoteAssetState* State = RemoteAssetStates.Find(Key);
	if (!State)
	{
		return;
	}

	// Unknown type: fail fast.
	const FString ObjectKey = MakeObjectKey(Key);
	if (ObjectKey.IsEmpty())
	{
		UE_LOG(LogTemp, Error,
		       TEXT("AssetManager: Remote request has unknown type. Id=%s Type=%s"),
		       *Key.Id.ToString(), *Key.Type.ToString());
		FinishRemoteAssetKey(Key);
		if (Key.Type == FVAssetType::Mesh)
		{
			CompleteStaticMeshLoad(Key.Id, nullptr);
		}
		return;
	}

	// Respect max attempts.
	if (State->AttemptsMade >= FMath::Max(1, State->MaxAttempts))
	{
		UE_LOG(LogTemp, Error,
		       TEXT("AssetManager: Remote request exhausted attempts. Id=%s Type=%s Attempts=%d"),
		       *Key.Id.ToString(), *Key.Type.ToString(), State->AttemptsMade);
		FinishRemoteAssetKey(Key);
		if (Key.Type == FVAssetType::Mesh)
		{
			CompleteStaticMeshLoad(Key.Id, nullptr);
		}
		return;
	}

	// Start an attempt.
	State->bInProgress = true;
	ActiveAssetRequests++;
	State->AttemptsMade++;

	const FString LocalPath = MakeLocalCachePath(ObjectKey);
	const bool bFileExists = FPlatformFileManager::Get().GetPlatformFile().FileExists(*LocalPath);

	auto CompleteFailure = [this, Key]()
	{
		// Mesh requests are coalesced via PendingStaticMeshLoads; ensure we always drain callbacks.
		if (Key.Type == FVAssetType::Mesh)
		{
			CompleteStaticMeshLoad(Key.Id, nullptr);
		}
	};

	auto RetryOrFail = [this, Key, ObjectKey, CompleteFailure](const TCHAR* Reason)
	{
		FVRemoteAssetState* RetryState = RemoteAssetStates.Find(Key);
		if (!RetryState)
		{
			FinishRemoteAssetKey(Key);
			CompleteFailure();
			return;
		}

		UE_LOG(LogTemp, Warning,
		       TEXT("AssetManager: Remote request attempt failed (%s). Id=%s Type=%s Attempt=%d/%d"),
		       Reason,
		       *Key.Id.ToString(),
		       *Key.Type.ToString(),
		       RetryState->AttemptsMade,
		       RetryState->MaxAttempts);

		// If we have attempts left, re-queue; otherwise, fail.
		if (RetryState->AttemptsMade < RetryState->MaxAttempts)
		{
			RetryState->bInProgress = false;
			ActiveAssetRequests = FMath::Max(0, ActiveAssetRequests - 1);

			// Ensure corrupted / partial files do not poison retries.
			const FString LocalRetryPath = MakeLocalCachePath(ObjectKey);
			IFileManager::Get().Delete(*LocalRetryPath, false, true, true);

			EnqueueRemoteAssetRequest(Key, RetryState->Name);
			return;
		}

		FinishRemoteAssetKey(Key);
		CompleteFailure();
	};

	// Asset-specific continuation after we have a local file.
	auto ContinueWithLocalFile = [this, Key, ObjectKey, RetryOrFail]()
	{
		if (Key.Type == FVAssetType::Mesh)
		{
			// We only ever call this from a mesh request path, so Name must be set.
			FVRemoteAssetState* LocalState = RemoteAssetStates.Find(Key);
			const FString MeshName = LocalState ? LocalState->Name.ToString() : FString();
			if (MeshName.IsEmpty())
			{
				RetryOrFail(TEXT("MissingName"));
				return;
			}

			const FString PakPath = MakeLocalCachePath(ObjectKey);
			if (!FVPAKUtility::Mount(PakPath, Key.Id.ToString()))
			{
				RetryOrFail(TEXT("MountFailed"));
				return;
			}

			const FString AssetKey = FString::Printf(TEXT("/Game/%s/%s.%s"), *Key.Id.ToString(), *MeshName, *MeshName);
			FVPAKUtility::LoadPAKAsync<UStaticMesh>(AssetKey, [this, Key, PakPath, ObjectKey, RetryOrFail](UStaticMesh* Loaded)
			{
				// Always unmount after load completes.
				if (FPakPlatformFile* PakPlatform = static_cast<FPakPlatformFile*>(FPlatformFileManager::Get().FindPlatformFile(TEXT("PakFile"))))
				{
					PakPlatform->Unmount(*PakPath);
				}

				if (!Loaded)
				{
					RetryOrFail(TEXT("LoadFailed"));
					return;
				}

				// Success: complete waiting callbacks.
				FinishRemoteAssetKey(Key);
				CompleteStaticMeshLoad(Key.Id, Loaded);
			});
			return;
		}

		if (Key.Type == FVAssetType::Texture)
		{
			FVRemoteAssetState* LocalState = RemoteAssetStates.Find(Key);
			const bool bLinear = LocalState ? LocalState->bLinear : false;

			const FString TexturePath = MakeLocalCachePath(ObjectKey);
			// Decode CRN on a worker thread, create UTexture2D on the game thread.
			Async(EAsyncExecution::ThreadPool, [this, Key, bLinear, TexturePath, RetryOrFail]()
			{
				TArray<uint8> TextureData;
				if (!FFileHelper::LoadFileToArray(TextureData, *TexturePath))
				{
					AsyncTask(ENamedThreads::GameThread, [RetryOrFail]() { RetryOrFail(TEXT("ReadFailed")); });
					return;
				}

				TArray<uint8> DDS = FVCrunchUtility::CRN_To_DDS(TextureData, false);
				AsyncTask(ENamedThreads::GameThread, [this, Key, bLinear, DDS = MoveTemp(DDS), RetryOrFail]() mutable
				{
					UTexture2D* Tex = FVCrunchUtility::LoadTextureDDS(DDS, !bLinear, true, false);
					if (!Tex)
					{
						RetryOrFail(TEXT("DecodeFailed"));
						return;
					}

					FinishRemoteAssetKey(Key);
					CompleteTextureLoad(Key.Id, Tex);
				});
			});
			return;
		}

		// Unknown type continuation.
		RetryOrFail(TEXT("UnsupportedType"));
	};

	if (bFileExists)
	{
		// Local file already present: go directly to the load stage.
		ContinueWithLocalFile();
		return;
	}

	// Download to cache first.
	if (CachePath.IsEmpty() || CDN_URL.IsEmpty())
	{
		RetryOrFail(TEXT("MissingCacheOrCDNConfig"));
		return;
	}

	UE_LOG(LogTemp, Log,
	       TEXT("AssetManager: Downloading remote asset. Type=%s Id=%s Key=%s Attempt=%d/%d"),
	       *Key.Type.ToString(),
	       *Key.Id.ToString(),
	       *ObjectKey,
	       State->AttemptsMade,
	       State->MaxAttempts);

	FVHTTPUtility::DownloadObjectAsync(CachePath, CDN_URL, CDN_Password, ObjectKey,
		[this, ContinueWithLocalFile, RetryOrFail](bool bSuccess)
		{
			if (!bSuccess)
			{
				RetryOrFail(TEXT("DownloadFailed"));
				return;
			}
			ContinueWithLocalFile();
		});
}

void UVAssetManager::FinishRemoteAssetKey(const FVAssetKey& Key)
{
	FVRemoteAssetState* State = RemoteAssetStates.Find(Key);
	if (State)
	{
		if (State->bInProgress)
		{
			ActiveAssetRequests = FMath::Max(0, ActiveAssetRequests - 1);
		}
		State->bInProgress = false;
	}
	RemoteAssetStates.Remove(Key);
	PumpRemoteAssetQueue();
}

// Initialize the asset cache manager
bool UVAssetManager::Initialize(const FString& InCachePath, bool InEmptyCache, const FString& InAssetServerURL, const FString& InAssetServerPassword, bool InServerMode) const
{
	// Globals
	CachePath = InCachePath;
	CDN_URL = InAssetServerURL;
	CDN_Password = InAssetServerPassword;

	// Initialize components
	if (!MaterialAgent->Initialize(InServerMode))
		return false;
	if (!TextureAgent->Initialize())
		return false;
	if (!MeshAgent->Initialize())
		return false;
	if (!SkeletalAgent->Initialize())
		return false;
	if (!AudioAgent->Initialize())
		return false;
	if (!LightAgent->Initialize())
		return false;

	// Set the cache path (create if not exist)

	if (SetCachePath(CachePath))
	{
		if (InEmptyCache)
			EmptyCache();

		UE_LOG(LogTemp, Log, TEXT("Cache Manager - Initialize [ Success ]"));

		return true;
	}

	UE_LOG(LogTemp, Log, TEXT("Cache Manager - Initialize [ Failed ]"));

	return false;
}

// Set the cache path - creates if unavailable
bool UVAssetManager::SetCachePath(const FString& InPath)
{
	// Create the cache path if it does not already exist
	if (!FPaths::DirectoryExists(InPath))
	{
		// Create the cached asset directory
		if (!IFileManager::Get().MakeDirectory(*InPath, true))
			return false;
	}

	CachePath = InPath;
	return true;
}

// Empty the cache (memory and disk)
void UVAssetManager::EmptyCache() const
{
	// Delete all files inside folder
	IFileManager::Get().DeleteDirectory(*CachePath, false, true);

	// Remove all the shared memory
	MeshAgent->RemoveAll();
	MaterialAgent->RemoveAll();
}


// Is the asset cached locally (disk)
bool UVAssetManager::IsDiskCached(const FGuid& InAssetID, const FName& InAssetType) const
{
	if (InAssetType == FVAssetType::Mesh)
	{
		if (MeshAgent->IsLocallyCached(InAssetID, FVCacheType::Disk))
			return true;
	}

	if (InAssetType == FVAssetType::Texture)
	{
		if (TextureAgent->IsLocallyCached(InAssetID, FVCacheType::Disk))
			return true;
	}

	return false;
}


// Is asset cached in memory
bool UVAssetManager::IsMemoryCached(const FGuid& InAssetID, const FName& InAssetType) const
{
	// Check Memory Status
	if (InAssetType == FVAssetType::Material)
	{
		if (MaterialAgent->IsMaterialCached(InAssetID))
			return true;
	}

	if (InAssetType == FVAssetType::Mesh)
	{
		if (MeshAgent->IsLocallyCached(InAssetID, FVCacheType::Memory))
			return true;
	}

	if (InAssetType == FVAssetType::Texture)
	{
		if (TextureAgent->IsLocallyCached(InAssetID, FVCacheType::Memory))
			return true;
	}

	return false;
}

// Update the material item with textures
void UVAssetManager::UpdateMaterialItem(const FVMMaterial& InMaterialItem) const
{
	if (UMaterialInstanceDynamic* MaterialInstance = MaterialAgent->GetMaterial(InMaterialItem.id))
	{
		for (const FVMTexture& TextureItem : InMaterialItem.textures)
		{
			if (UTexture2D* Texture = TextureAgent->GetTexture(TextureItem.asset_ref.id))
				MaterialInstance->SetTextureParameterValue(TextureItem.property, Texture);
		}
	}
}

void UVAssetManager::CompleteStaticMeshLoad(const FGuid& InMeshId, UStaticMesh* InOutMesh)
{
	TArray<FVOnStaticMeshLoaded> Callbacks;
	if (!PendingStaticMeshLoads.RemoveAndCopyValue(InMeshId, Callbacks))
	{
		return;
	}

	if (InOutMesh && MeshAgent)
	{
		MeshAgent->SetMesh(InMeshId, InOutMesh);
	}

	for (FVOnStaticMeshLoaded& CB : Callbacks)
	{
		if (CB.IsBound())
		{
			CB.Execute(InOutMesh);
		}
	}
}

void UVAssetManager::CompleteTextureLoad(const FGuid& InTextureId, UTexture2D* InOutTexture)
{
	TArray<FVOnTextureLoaded> Callbacks;
	if (!PendingTextureLoads.RemoveAndCopyValue(InTextureId, Callbacks))
	{
		return;
	}

	if (InOutTexture && TextureAgent)
	{
		TextureAgent->SetTexture(InTextureId, InOutTexture);
	}

	for (FVOnTextureLoaded& CB : Callbacks)
	{
		if (CB.IsBound())
		{
			CB.Execute(InOutTexture);
		}
	}
}

void UVAssetManager::CompleteMaterialLoad(const FGuid& InMaterialId, UMaterialInstanceDynamic* InOutMaterial)
{
	TArray<FVOnMaterialInstanceLoaded> Callbacks;
	if (!PendingMaterialLoads.RemoveAndCopyValue(InMaterialId, Callbacks))
	{
		return;
	}

	if (InOutMaterial && MaterialAgent)
	{
		MaterialAgent->SetMaterial(InMaterialId, InOutMaterial);
	}

	for (FVOnMaterialInstanceLoaded& CB : Callbacks)
	{
		if (CB.IsBound())
		{
			CB.Execute(InOutMaterial);
		}
	}
}

void UVAssetManager::RequestStaticMeshAsync(
	const FVMMeshComponentNet& InMesh,
	FVOnStaticMeshLoaded OutCallback)
{
	const FGuid MeshId = InMesh.mesh_ref.id.Value;

	UE_LOG(LogTemp, Verbose,
	       TEXT("AssetManager: RequestStaticMeshAsync MeshId=%s"),
	       *MeshId.ToString());

	if (!MeshId.IsValid())
	{
		UE_LOG(LogTemp, Warning,
		       TEXT("AssetManager: Invalid MeshId"));
		if (OutCallback.IsBound())
		{
			OutCallback.Execute(nullptr);
		}
		return;
	}

	// --------------------
	// Memory cache
	// --------------------
	if (MeshAgent)
	{
		if (UStaticMesh* Cached = MeshAgent->GetMesh(MeshId))
		{
			/*UE_LOG(LogTemp, Verbose,
			       TEXT("AssetManager: Cache HIT MeshId=%s"),
			       *MeshId.ToString());*/

			if (OutCallback.IsBound())
			{
				OutCallback.Execute(Cached);
			}
			return;
		}
	}

	/*UE_LOG(LogTemp, Verbose,
	       TEXT("AssetManager: Cache MISS MeshId=%s"),
	       *MeshId.ToString());*/

	// --------------------
	// Coalesce concurrent requests
	// --------------------
	if (TArray<FVOnStaticMeshLoaded>* Existing = PendingStaticMeshLoads.Find(MeshId))
	{
		/*UE_LOG(LogTemp, Verbose,
		       TEXT("AssetManager: Coalescing load MeshId=%s"),
		       *MeshId.ToString());*/

		Existing->Add(MoveTemp(OutCallback));
		return;
	}

	PendingStaticMeshLoads.Add(MeshId, {MoveTemp(OutCallback)});

#if WITH_EDITOR
	if (ShouldUseEditorAssetPathResolution())
	{
		const FSoftObjectPath SoftPath = MakeSoftObjectPathFromAssetRef(InMesh.mesh_ref.path, InMesh.mesh_ref.name);
		if (SoftPath.IsNull())
		{
			UE_LOG(LogTemp, Warning,
			       TEXT("AssetManager: PIE mesh_ref.path empty/invalid for MeshId=%s"),
			       *MeshId.ToString());
			CompleteStaticMeshLoad(MeshId, nullptr);
			return;
		}

		FVPAKUtility::LoadAssetAsync<UStaticMesh>(
			SoftPath,
			[this, MeshId](UStaticMesh* Loaded)
			{
				UE_LOG(LogTemp, Log,
				       TEXT("AssetManager: PIE mesh load %s MeshId=%s"),
				       Loaded ? TEXT("SUCCESS") : TEXT("FAILED"),
				       *MeshId.ToString());

				CompleteStaticMeshLoad(MeshId, Loaded);
			});
		return;
	}
#endif

	// --------------------
	// Packaged path
	// --------------------
	const FString MeshName = InMesh.mesh_ref.name.ToString();
	if (MeshName.IsEmpty())
	{
		UE_LOG(LogTemp, Error,
		       TEXT("AssetManager: MeshName empty for MeshId=%s"),
		       *MeshId.ToString());

		CompleteStaticMeshLoad(MeshId, nullptr);
		return;
	}

	// Packaged builds resolve via cache + optional remote acquisition.
	// This call is throttled and retried via the AssetManager's remote request queue.
	const FVAssetKey Key(MeshId, FVAssetType::Mesh);
	EnqueueRemoteAssetRequest(Key, InMesh.mesh_ref.name);
}

bool UVAssetManager::ShouldUseEditorAssetPathResolution() const
{
#if WITH_EDITOR
	// In editor builds we prefer resolving authored asset refs directly from the editor asset registry.
	// This is intentionally gated by GIsEditor (as requested) rather than by WorldType.
	return GIsEditor;
#else
	return false;
#endif
}

FSoftObjectPath UVAssetManager::MakeSoftObjectPathFromAssetRef(const FName& InPath, const FName& InName)
{
	const FString PathStr = InPath.ToString();
	if (PathStr.IsEmpty())
	{
		return FSoftObjectPath();
	}

	FString ObjectPath = PathStr;
	if (!ObjectPath.Contains(TEXT(".")))
	{
		FString NameStr = InName != NAME_None ? InName.ToString() : FString();
		if (NameStr.IsEmpty())
		{
			NameStr = FPaths::GetBaseFilename(ObjectPath);
		}
		if (!NameStr.IsEmpty())
		{
			ObjectPath += TEXT(".") + NameStr;
		}
	}

	return FSoftObjectPath(ObjectPath);
}

void UVAssetManager::RequestTextureAsync(const FVMTexture& InTexture, FVOnTextureLoaded OutCallback)
{
	const FGuid TextureId = InTexture.asset_ref.id;

	if (!TextureId.IsValid())
	{
		if (OutCallback.IsBound())
		{
			OutCallback.Execute(nullptr);
		}
		return;
	}

	// Memory cache
	if (TextureAgent)
	{
		if (UTexture2D* Cached = TextureAgent->GetTexture(TextureId))
		{
			if (OutCallback.IsBound())
			{
				OutCallback.Execute(Cached);
			}
			return;
		}
	}

	// Coalesce concurrent requests
	if (TArray<FVOnTextureLoaded>* Existing = PendingTextureLoads.Find(TextureId))
	{
		Existing->Add(MoveTemp(OutCallback));
		return;
	}
	PendingTextureLoads.Add(TextureId, { MoveTemp(OutCallback) });

#if WITH_EDITOR
	if (ShouldUseEditorAssetPathResolution() && InTexture.asset_ref.path != NAME_None)
	{
		const FSoftObjectPath SoftPath = MakeSoftObjectPathFromAssetRef(InTexture.asset_ref.path, InTexture.asset_ref.name);
		if (SoftPath.IsNull())
		{
			UE_LOG(LogTemp, Warning,
			       TEXT("AssetManager: PIE texture.path invalid for TextureId=%s"),
			       *TextureId.ToString());
			CompleteTextureLoad(TextureId, nullptr);
			return;
		}

		FVPAKUtility::LoadAssetAsync<UTexture2D>(SoftPath,
			[this, TextureId](UTexture2D* Loaded)
			{
				CompleteTextureLoad(TextureId, Loaded);
			});
		return;
	}
#endif

	// Disk-cache path (packaged runtime)
	const FString TextureFile = FPaths::Combine(CachePath, FString::Printf(TEXT("%s.texture"), *TextureId.ToString()));
	if (!TextureFile.IsEmpty() && FPlatformFileManager::Get().GetPlatformFile().FileExists(*TextureFile))
	{
		const bool bLinear = InTexture.linear;
		Async(EAsyncExecution::ThreadPool, [this, TextureId, TextureFile, bLinear]()
		{
			TArray<uint8> TextureData;
			if (!FFileHelper::LoadFileToArray(TextureData, *TextureFile))
			{
				AsyncTask(ENamedThreads::GameThread, [this, TextureId]()
				{
					CompleteTextureLoad(TextureId, nullptr);
				});
				return;
			}

			TArray<uint8> DDS = FVCrunchUtility::CRN_To_DDS(TextureData, false);
			AsyncTask(ENamedThreads::GameThread, [this, TextureId, bLinear, DDS = MoveTemp(DDS)]() mutable
			{
				UTexture2D* Tex = FVCrunchUtility::LoadTextureDDS(DDS, !bLinear, true, false);
				CompleteTextureLoad(TextureId, Tex);
			});
		});
		return;
	}

	// Remote acquisition (packaged runtime)
	const FVAssetKey Key(TextureId, FVAssetType::Texture);
	EnqueueRemoteAssetRequest(Key, InTexture.asset_ref.name, InTexture.linear);
}

void UVAssetManager::RequestMaterialInstanceAsync(const FGuid& InMaterialId, FVOnMaterialInstanceLoaded OutCallback)
{
	if (!InMaterialId.IsValid())
	{
		if (OutCallback.IsBound())
		{
			OutCallback.Execute(nullptr);
		}
		return;
	}

	if (MaterialAgent)
	{
		if (UMaterialInstanceDynamic* Cached = MaterialAgent->GetMaterial(InMaterialId))
		{
			if (OutCallback.IsBound())
			{
				OutCallback.Execute(Cached);
			}
			return;
		}
	}

	if (TArray<FVOnMaterialInstanceLoaded>* Existing = PendingMaterialLoads.Find(InMaterialId))
	{
		Existing->Add(MoveTemp(OutCallback));
		return;
	}
	PendingMaterialLoads.Add(InMaterialId, { MoveTemp(OutCallback) });
	TryResolveMaterialInstance(InMaterialId);
}

void UVAssetManager::SubmitMaterialItems(const TArray<FVMMaterial>& InMaterials)
{
	if (!MaterialAgent)
	{
		return;
	}

	for (const FVMMaterial& Mat : InMaterials)
	{
		MaterialAgent->AddMaterialItem(Mat);
		TryResolveMaterialInstance(Mat.id);
	}
}

void UVAssetManager::TryResolveMaterialInstance(const FGuid& InMaterialId)
{
	if (!InMaterialId.IsValid() || !MaterialAgent)
	{
		return;
	}

	// If already created, just complete pending callbacks.
	if (UMaterialInstanceDynamic* Existing = MaterialAgent->GetMaterial(InMaterialId))
	{
		CompleteMaterialLoad(InMaterialId, Existing);
		return;
	}

	FVMMaterial MaterialItem;
	if (!MaterialAgent->GetMaterialItem(InMaterialId, MaterialItem))
	{
		// Material item not yet received; leave callbacks pending.
		return;
	}

	UMaterialInstanceDynamic* MID = MaterialAgent->CreateMaterial(InMaterialId, MaterialItem.parent);
	if (!MID)
	{
		CompleteMaterialLoad(InMaterialId, nullptr);
		return;
	}

	// Scalars / vectors are immediate.
	for (const FVMScalar& ScalarItem : MaterialItem.scalars)
	{
		MID->SetScalarParameterValue(ScalarItem.name, ScalarItem.value);
	}
	for (const FVMLinearColor& VectorItem : MaterialItem.vectors)
	{
		MID->SetVectorParameterValue(VectorItem.name, VectorItem.color);
	}

	// Resolve dependent textures asynchronously and patch the MID as they arrive.
	for (const FVMTexture& TexItem : MaterialItem.textures)
	{
		RequestTextureAsync(TexItem, FVOnTextureLoaded::CreateLambda(
			[this, InMaterialId, Property = TexItem.property](UTexture2D* LoadedTex)
			{
				if (!LoadedTex || !MaterialAgent)
				{
					return;
				}
				if (UMaterialInstanceDynamic* LiveMID = MaterialAgent->GetMaterial(InMaterialId))
				{
					LiveMID->SetTextureParameterValue(Property, LoadedTex);
				}
			}));
	}

	// Complete immediately so meshes can bind the MID while textures stream in.
	CompleteMaterialLoad(InMaterialId, MID);
}


/*
// HTTP synchronous asset downloader
bool UCacheManager::DownloadAssetSync(const FMAssetRef& InAssetRequest) const
{
	if (!InAssetRequest.id.IsValid())
		return false;

	return FHTTPLoader::DownloadSync(CachePath, AssetServerURL, AssetServerPassword, InAssetRequest.GetAssetKey());
}

// HTTP asynchronous asset downloader with callback
void UCacheManager::DownloadAssetAsync(const FString& InObjectKey, TFunction<void(bool)> Callback) const
{
	// Call the http download
	FHTTPLoader::DownloadAsync(CachePath, AssetServerURL, AssetServerPassword, InObjectKey, [Callback](const bool DownloadSuccess)
	{
		Callback(DownloadSuccess);
	});
}

// Load PAK mesh asset asynchronously
void UCacheManager::LoadMeshPAKAsync(const FMeshAssetRequest& InMeshAssetRequest, TFunction<void(UStaticMesh*)> Callback) const
{
	const FString PAKPath = FPaths::Combine(CachePath, InMeshAssetRequest.AssetRef.GetAssetKey());
	if (!FAssetLoader::Mount(PAKPath, InMeshAssetRequest.AssetRef.id.ToString()))
	{
		UE_LOG(LogTemp, Error, TEXT("Cache Manager - Load PAK Async [ Mount error, PAKPath: %s ]"), *PAKPath);
		Callback(nullptr);
		return;
	}

	// Create the asset key
	FString AssetKey = FString::Printf(TEXT("/Game/%s/%s.%s"), *InMeshAssetRequest.AssetRef.id.ToString(), *InMeshAssetRequest.AssetRef.name.ToString(), *InMeshAssetRequest.AssetRef.name.ToString());

	FAssetLoader::LoadPAKAsync<UStaticMesh>(AssetKey, [InMeshAssetRequest, Callback, this](UStaticMesh* Mesh)
	{
		if (Mesh)
		{
			// Only unmount for standalone
			const FString PakPath = FPaths::Combine(CachePath, InMeshAssetRequest.AssetRef.GetAssetKey());
			if (FPakPlatformFile* PakPlatform = static_cast<FPakPlatformFile*>(FPlatformFileManager::Get().FindPlatformFile(TEXT("PakFile"))))
			{
				if (!PakPlatform->Unmount(*PakPath))
				{
					UE_LOG(LogTemp, Warning, TEXT("Cache Manager - Load PAK Async [ Unmount error, PAKPath: %s ]"), *PakPath);
				}
			}
		}
		
		Callback(Mesh);
	});
}

// Load PAK mesh asset synchronously
bool UCacheManager::LoadMeshPAKSync(const FMeshAssetRequest& InMeshAssetRequest) const
{
	// Only mount in non-editor builds
	if (!GIsEditor)
	{
		const FString PAKPath = FPaths::Combine(CachePath, InMeshAssetRequest.AssetRef.GetAssetKey());
		if (!FAssetLoader::Mount(PAKPath, InMeshAssetRequest.AssetRef.id.ToString()))
		{
			UE_LOG(LogTemp, Error, TEXT("Asset Service - Load Asset Async [ Mount Error, PAK Path: %s"), *PAKPath);
			return false;
		}
	}

	if (UStaticMesh* StaticMesh = FAssetLoader::LoadPAKSync<UStaticMesh>(InMeshAssetRequest.AssetRef.id, InMeshAssetRequest.AssetRef.name))
	{
		MeshComp->SetMesh(InMeshAssetRequest.AssetRef.id, StaticMesh);

		// Remove asset from the root
		StaticMesh->RemoveFromRoot();

		// Mark the asset for garbage
		StaticMesh->MarkAsGarbage();

		// Garbage collect before un-mount
		CollectGarbage(GARBAGE_COLLECTION_KEEPFLAGS);

		// Now unmount safely
		const FString PakPath = FPaths::Combine(CachePath, InMeshAssetRequest.AssetRef.id.ToString(), TEXT("/"));
		if (FPakPlatformFile* PakPlatform = static_cast<FPakPlatformFile*>(FPlatformFileManager::Get().FindPlatformFile(TEXT("PakFile"))))
		{
			PakPlatform->Unmount(*PakPath);
		}

		UE_LOG(LogTemp, Log, TEXT("Cache Manager - Load Mesh PAK Sync [ Unmount Failed, PAK Path: %s ]"), *PakPath);

		return true;
	}

	UE_LOG(LogTemp, Error, TEXT("Cache Manager - Load Mesh Asset Sync [ PAK load error, Asset ID: %s, Asset Name: %s"),
	       *InMeshAssetRequest.AssetRef.id.ToString(), *InMeshAssetRequest.AssetRef.name.ToString());

	return false;
}*/