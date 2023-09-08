// Fill out your copyright notice in the Description page of Project Settings.


#include "KKHelperLibrary.h"


// com lib start  : TComPtr
#include "Windows/COMPointer.h"
// com lib end

// debug hit result start 
#include "PhysicalMaterials/PhysicalMaterial.h"
// debug hit result end




#include "Windows/AllowWindowsPlatformTypes.h"

#include <ShObjIdl_core.h>

#include "Windows/HideWindowsPlatformTypes.h"






FVector UKKHelperLibrary::Lib_GetRandomPosInRandomRange(float MinRange, float MaxRange)
{
	return FMath::VRand() * FMath::RandRange(MinRange, MaxRange);
}

TArray<FVector> UKKHelperLibrary::Lib_GetRandomDirAroundAxis(FVector InitDir, FVector AroundAxis, int32 Count)
{
	TArray<FVector> List;
	float PerAngle = 360.f / Count;
	for (int32 i = 0; i < Count; ++i)
	{
		List.Emplace(InitDir.RotateAngleAxis(PerAngle * i, AroundAxis));
	}
	return List;
}

FTransform& UKKHelperLibrary::Lib_ChangeTransformScale(FTransform& InTransform, FVector NewScale)
{
	InTransform.SetScale3D(NewScale);
	return InTransform;
}

void UKKHelperLibrary::Lib_DebugStringOnFixedPos(const FString& InContent, int32 FixPos, float Time, FColor Color)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(FixPos, Time, Color, InContent);
	}
}

void UKKHelperLibrary::Lib_DebugHitResult(const FHitResult& HitResult, bool bShowOnScreen, float Time)
{
	FString DebugString = FString::Printf(
		TEXT(
			"BlockingHit:%d | InitialOverlap:%d | Time:%f | Distance:%f | Location:%s | ImpactPoint:%s | Normal:%s | ImpactNormal:%s | PhysMatName:%s | HitActorName:%s | HitComponent:%s | HitBoneName:%s | HitItem:%d | FaceIndex:%d | TraceStart:%s | TraceEnd:%s"),
		HitResult.bBlockingHit, HitResult.bStartPenetrating, HitResult.Time, HitResult.Distance,
		*HitResult.Location.ToString(), *HitResult.ImpactPoint.ToString(), *HitResult.Normal.ToString(),
		*HitResult.ImpactNormal.ToString(),
		HitResult.PhysMaterial.IsValid() ? *HitResult.PhysMaterial.Get()->GetName() : TEXT("NULL"),
		HitResult.GetActor() ? *HitResult.GetActor()->GetName() : TEXT("NULL"),
		HitResult.GetComponent() ? *HitResult.GetComponent()->GetName() : TEXT("NULL"),
		*HitResult.BoneName.ToString(), HitResult.Item, HitResult.FaceIndex, *HitResult.TraceStart.ToString(),
		*HitResult.TraceEnd.ToString());

	if (bShowOnScreen && GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, Time, FColor::Red, DebugString);
	}
	UE_LOG(LogTemp, Warning, TEXT("[HelperLib:51]HitResult:%s"), *DebugString);
}

FString UKKHelperLibrary::Lib_GetNetModeString(ENetMode NetMode)
{
	switch (NetMode)
	{
	case NM_Client:
		return TEXT("NM_Client");
	case NM_Standalone:
		return TEXT("NM_Standalone");
	case NM_DedicatedServer:
		return TEXT("NM_DedicatedServer");
	case NM_ListenServer:
		return TEXT("NM_ListenServer");
	default:
		return TEXT("ErrorNetMode");
	}
}

FString UKKHelperLibrary::Lib_GetRoleString(ENetRole NetRole)
{
	switch (NetRole)
	{
	case ROLE_Authority:
		return TEXT("ROLE_Authority");
	case ROLE_AutonomousProxy:
		return TEXT("ROLE_AutonomousProxy");
	case ROLE_SimulatedProxy:
		return TEXT("ROLE_SimulatedProxy");
	case ROLE_None:
		return TEXT("ROLE_None");
	default:
		return TEXT("ErrorRole");
	}
}

FActorSpawnParameters UKKHelperLibrary::Lib_GetActorAlwaysSpawnParameter()
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	return SpawnParameters;
}

bool UKKHelperLibrary::Lib_OpenFileDialog(const FString& DialogTitle, const FString& DefaultPath, const FString& DefaultFile,
                                    const FString& FileTypes, bool bMultiSelect, TArray<FString>& OutFilenames)
{
	
	const void * ParentWindowHandle = FSlateApplication::Get().FindBestParentWindowHandleForDialogs(nullptr);
	uint32 Flags = bMultiSelect ? 0x01:0x00;
	int32 Tmp;
	return CF_FileDialogShared(false,ParentWindowHandle,DialogTitle,DefaultPath,DefaultFile,FileTypes,Flags,OutFilenames,Tmp);
}

bool UKKHelperLibrary::Lib_OpenFileDialogWithIndex(const FString& DialogTitle, const FString& DefaultPath,
	const FString& DefaultFile, const FString& FileTypes, bool bMultiSelect, TArray<FString>& OutFilenames,
	int32& OutFilterIndex)
{
	const void * ParentWindowHandle = FSlateApplication::Get().FindBestParentWindowHandleForDialogs(nullptr);
	uint32 Flags = bMultiSelect ? 0x01:0x00;
	return CF_FileDialogShared(false,ParentWindowHandle,DialogTitle,DefaultPath,DefaultFile,FileTypes,Flags,OutFilenames,OutFilterIndex);
}

bool UKKHelperLibrary::Lib_SaveFileDialog(const FString& DialogTitle, const FString& DefaultPath, const FString& DefaultFile,
	const FString& FileTypes, bool bMultiSelect, TArray<FString>& OutFilenames)
{
	const void * ParentWindowHandle = FSlateApplication::Get().FindBestParentWindowHandleForDialogs(nullptr);
	uint32 Flags = bMultiSelect ? 0x01:0x00;
	int32 DummyFilterIndex = 0;
	return CF_FileDialogShared(true, ParentWindowHandle, DialogTitle, DefaultPath, DefaultFile, FileTypes, Flags, OutFilenames, DummyFilterIndex );
}

bool UKKHelperLibrary::Lib_SaveFileDialogSimple(const FString& DialogTitle, const FString& DefaultPath,
	const FString& FileTypes, FString& OutFilename)
{
	const void * ParentWindowHandle = FSlateApplication::Get().FindBestParentWindowHandleForDialogs(nullptr);
	uint32 Flags = 0x00;
	int32 DummyFilterIndex = 0;
	TArray<FString> Paths;
	bool ret = CF_FileDialogShared(true, ParentWindowHandle, DialogTitle, DefaultPath, TEXT(""), FileTypes, Flags, Paths, DummyFilterIndex );
	OutFilename = Paths.Num() ? Paths[0] : TEXT("Error");
	if (Paths.Num() <= 0)
	{
		ret = false;
	}
	return ret;
}

bool UKKHelperLibrary::Lib_OpenDirectoryDialog(const FString& DialogTitle, const FString& DefaultPath, FString& OutFolderName)
{
	const void * ParentWindowHandle = FSlateApplication::Get().FindBestParentWindowHandleForDialogs(nullptr);
	// FScopedSystemModalMode SystemModalScope;
	
	bool bSuccess = false;

	TComPtr<IFileOpenDialog> FileDialog;
	if (SUCCEEDED(::CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&FileDialog))))
	{
		// Set this up as a folder picker
		{
			DWORD dwFlags = 0;
			FileDialog->GetOptions(&dwFlags);
			FileDialog->SetOptions(dwFlags | FOS_PICKFOLDERS);
		}

		// Set up common settings
		FileDialog->SetTitle(*DialogTitle);
		if (!DefaultPath.IsEmpty())
		{
			// SHCreateItemFromParsingName requires the given path be absolute and use \ rather than / as our normalized paths do
			FString DefaultWindowsPath = FPaths::ConvertRelativePathToFull(DefaultPath);
			DefaultWindowsPath.ReplaceInline(TEXT("/"), TEXT("\\"), ESearchCase::CaseSensitive);

			TComPtr<IShellItem> DefaultPathItem;
			if (SUCCEEDED(::SHCreateItemFromParsingName(*DefaultWindowsPath, nullptr, IID_PPV_ARGS(&DefaultPathItem))))
			{
				FileDialog->SetFolder(DefaultPathItem);
			}
		}

		// Show the picker
		if (SUCCEEDED(FileDialog->Show((HWND)ParentWindowHandle)))
		{
			TComPtr<IShellItem> Result;
			if (SUCCEEDED(FileDialog->GetResult(&Result)))
			{
				PWSTR pFilePath = nullptr;
				if (SUCCEEDED(Result->GetDisplayName(SIGDN_FILESYSPATH, &pFilePath)))
				{
					bSuccess = true;

					OutFolderName = pFilePath;
					FPaths::NormalizeDirectoryName(OutFolderName);

					::CoTaskMemFree(pFilePath);
				}
			}
		}
	}
	
	return bSuccess;
}

bool UKKHelperLibrary::Lib_OpenFontDialog(FString& OutFontName, float& OutHeight, FFontImportFlagsBP & OutFlags)
{
	// FScopedSystemModalMode SystemModalScope;
	const void * ParentWindowHandle = FSlateApplication::Get().FindBestParentWindowHandleForDialogs(nullptr);
	
	CHOOSEFONT cf;
	ZeroMemory(&cf, sizeof(CHOOSEFONT));

	LOGFONT lf;
	ZeroMemory(&lf, sizeof(LOGFONT));

	cf.lStructSize = sizeof(CHOOSEFONT);
	cf.hwndOwner = (HWND)ParentWindowHandle;
	cf.lpLogFont = &lf;
	cf.Flags = CF_EFFECTS | CF_SCREENFONTS;
	bool bSuccess = !!::ChooseFont(&cf);
	if ( bSuccess )
	{
		HDC DC = ::GetDC( cf.hwndOwner ); 
		const float LogicalPixelsY = static_cast<float>(GetDeviceCaps(DC, LOGPIXELSY));
		const int32 PixelHeight = static_cast<int32>(-lf.lfHeight * ( 72.0f / LogicalPixelsY ));	// Always target 72 DPI
		auto FontFlags = EKKFontImportFlags::None;
		if ( lf.lfUnderline )
		{
			// FontFlags |= EFontImportFlagsBP::EnableUnderline;
			FontFlags |= EKKFontImportFlags::EnableUnderline;
		}
		if ( lf.lfItalic )
		{
			FontFlags |= EKKFontImportFlags::EnableItalic;
		}
		if ( lf.lfWeight == FW_BOLD )
		{
			FontFlags |= EKKFontImportFlags::EnableBold;
		}

		OutFontName = (const TCHAR*)lf.lfFaceName;
		OutHeight = PixelHeight;
		OutFlags.Flags = FontFlags;

		::ReleaseDC( cf.hwndOwner, DC ); 
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Error reading results of font dialog."));
	}

	return bSuccess;
}


bool UKKHelperLibrary::CF_FileDialogShared(bool bSave, const void* ParentWindowHandle, const FString& DialogTitle,
                                           const FString& DefaultPath, const FString& DefaultFile, const FString& FileTypes, uint32 Flags,
                                           TArray<FString>& OutFilenames, int32& OutFilterIndex)
{

	// FScopedSystemModalMode SystemModalScope;
	
	bool bSuccess = false;

	TComPtr<IFileDialog> FileDialog;
	if (SUCCEEDED(
		::CoCreateInstance(bSave ? CLSID_FileSaveDialog : CLSID_FileOpenDialog, nullptr, CLSCTX_INPROC_SERVER, bSave ?
			IID_IFileSaveDialog : IID_IFileOpenDialog, IID_PPV_ARGS_Helper(&FileDialog))))
	{
		if (bSave)
		{
			// Set the default "filename"
			if (!DefaultFile.IsEmpty())
			{
				FileDialog->SetFileName(*FPaths::GetCleanFilename(DefaultFile));
			}
		}
		else
		{
			// Set this up as a multi-select picker
			if (Flags & 0x01)
			{
				DWORD dwFlags = 0;
				FileDialog->GetOptions(&dwFlags);
				FileDialog->SetOptions(dwFlags | FOS_ALLOWMULTISELECT);
			}
		}

		// Set up common settings
		FileDialog->SetTitle(*DialogTitle);
		if (!DefaultPath.IsEmpty())
		{
			// SHCreateItemFromParsingName requires the given path be absolute and use \ rather than / as our normalized paths do
			FString DefaultWindowsPath = FPaths::ConvertRelativePathToFull(DefaultPath);
			DefaultWindowsPath.ReplaceInline(TEXT("/"), TEXT("\\"), ESearchCase::CaseSensitive);

			TComPtr<IShellItem> DefaultPathItem;
			if (SUCCEEDED(::SHCreateItemFromParsingName(*DefaultWindowsPath, nullptr, IID_PPV_ARGS(&DefaultPathItem))))
			{
				FileDialog->SetFolder(DefaultPathItem);
			}
		}

		// Set-up the file type filters
		TArray<FString> UnformattedExtensions;
		TArray<COMDLG_FILTERSPEC> FileDialogFilters;
		{
			// Split the given filter string (formatted as "Pair1String1|Pair1String2|Pair2String1|Pair2String2") into the Windows specific filter struct
			FileTypes.ParseIntoArray(UnformattedExtensions, TEXT("|"), true);

			if (UnformattedExtensions.Num() % 2 == 0)
			{
				FileDialogFilters.Reserve(UnformattedExtensions.Num() / 2);
				for (int32 ExtensionIndex = 0; ExtensionIndex < UnformattedExtensions.Num();)
				{
					COMDLG_FILTERSPEC& NewFilterSpec = FileDialogFilters[FileDialogFilters.AddDefaulted()];
					NewFilterSpec.pszName = *UnformattedExtensions[ExtensionIndex++];
					NewFilterSpec.pszSpec = *UnformattedExtensions[ExtensionIndex++];
				}
			}
		}
		FileDialog->SetFileTypes(FileDialogFilters.Num(), FileDialogFilters.GetData());

		// Show the picker
		if (SUCCEEDED(FileDialog->Show((HWND)ParentWindowHandle)))
		{
			OutFilterIndex = 0;
			if (SUCCEEDED(FileDialog->GetFileTypeIndex((UINT*)&OutFilterIndex)))
			{
				OutFilterIndex -= 1; // GetFileTypeIndex returns a 1-based index
			}

			auto AddOutFilename = [&OutFilenames](const FString& InFilename)
			{
				FString& OutFilename = OutFilenames[OutFilenames.Add(InFilename)];
				OutFilename = IFileManager::Get().ConvertToRelativePath(*OutFilename);
				FPaths::NormalizeFilename(OutFilename);
			};

			if (bSave)
			{
				TComPtr<IShellItem> Result;
				if (SUCCEEDED(FileDialog->GetResult(&Result)))
				{
					PWSTR pFilePath = nullptr;
					if (SUCCEEDED(Result->GetDisplayName(SIGDN_FILESYSPATH, &pFilePath)))
					{
						bSuccess = true;

						// Apply the selected extension if the given filename doesn't already have one
						FString SaveFilePath = pFilePath;
						if (FileDialogFilters.IsValidIndex(OutFilterIndex))
						{
							// May have multiple semi-colon separated extensions in the pattern
							const FString ExtensionPattern = FileDialogFilters[OutFilterIndex].pszSpec;
							TArray<FString> SaveExtensions;
							ExtensionPattern.ParseIntoArray(SaveExtensions, TEXT(";"));

							// Build a "clean" version of the selected extension (without the wildcard)
							FString CleanExtension = SaveExtensions[0];
							if (CleanExtension == TEXT("*.*"))
							{
								CleanExtension.Reset();
							}
							else
							{
								int32 WildCardIndex = INDEX_NONE;
								if (CleanExtension.FindChar(TEXT('*'), WildCardIndex))
								{
									CleanExtension.RightChopInline(WildCardIndex + 1, false);
								}
							}

							// We need to split these before testing the extension to avoid anything within the path being treated as a file extension
							FString SaveFileName = FPaths::GetCleanFilename(SaveFilePath);
							SaveFilePath = FPaths::GetPath(SaveFilePath);

							// Apply the extension if the file name doesn't already have one
							if (FPaths::GetExtension(SaveFileName).IsEmpty() && !CleanExtension.IsEmpty())
							{
								SaveFileName = FPaths::SetExtension(SaveFileName, CleanExtension);
							}

							SaveFilePath /= SaveFileName;
						}
						AddOutFilename(SaveFilePath);

						::CoTaskMemFree(pFilePath);
					}
				}
			}
			else
			{
				IFileOpenDialog* FileOpenDialog = static_cast<IFileOpenDialog*>(FileDialog.Get());

				TComPtr<IShellItemArray> Results;
				if (SUCCEEDED(FileOpenDialog->GetResults(&Results)))
				{
					DWORD NumResults = 0;
					Results->GetCount(&NumResults);
					for (DWORD ResultIndex = 0; ResultIndex < NumResults; ++ResultIndex)
					{
						TComPtr<IShellItem> Result;
						if (SUCCEEDED(Results->GetItemAt(ResultIndex, &Result)))
						{
							PWSTR pFilePath = nullptr;
							if (SUCCEEDED(Result->GetDisplayName(SIGDN_FILESYSPATH, &pFilePath)))
							{
								bSuccess = true;
								AddOutFilename(pFilePath);
								::CoTaskMemFree(pFilePath);
							}
						}
					}
				}
			}
		}
	}

	return bSuccess;
}
