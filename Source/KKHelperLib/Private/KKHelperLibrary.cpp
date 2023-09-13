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

#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"

#include "Components/LineBatchComponent.h"

#include "GameFramework/HUD.h"

#include "Windows/HideWindowsPlatformTypes.h"






FVector UKKHelperLibrary::Lib_GetRandomPosInRandomRange(float MinRange, float MaxRange)
{
	return FMath::VRand() * FMath::RandRange(MinRange, MaxRange);
}

FVector UKKHelperLibrary::Lib_GetEndPointByStartAndDirection(FVector Start, FVector Dir, float Length)
{
	return (Start + Dir * Length);
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

void UKKHelperLibrary::Lib_DebugStringOnFixedPos(const FString& InContent, int32 FixPos, float Time, FLinearColor Color)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(FixPos, Time, Color.ToFColor(true), InContent);
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

void UKKHelperLibrary::Lib_DrawDebugLineSupportShipping(UObject * WorldContextObject,FVector LineStart,FVector LineEnd,FLinearColor Color,bool bPersistent,float LifeTime,int32 DepthPriority,float Thickness)
{
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		if (GEngine->GetNetMode(World) != NM_DedicatedServer)
		{
			if (ULineBatchComponent* const LineBatcher = CF_GetDebugLineBatcher(World, bPersistent, LifeTime, (DepthPriority == SDPG_Foreground)))
			{
				float const LineLifeTime = CF_GetDebugLineLifeTime(LineBatcher, LifeTime, bPersistent);
				LineBatcher->DrawLine(LineStart, LineEnd, Color, DepthPriority, Thickness, LineLifeTime);
			}
		}
	}
}

void UKKHelperLibrary::Lib_DrawDebugCircleSupportShipping(const UObject* WorldContextObject, FVector Center,
	float Radius, int32 NumSegments, FLinearColor LineColor,bool bPersistent, float LifeTime, int32 DepthPriority, float Thickness, FVector YAxis,
	FVector ZAxis, bool bDrawAxis)
{
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		if (GEngine->GetNetMode(World) != NM_DedicatedServer)
		{
			if (ULineBatchComponent* const LineBatcher = CF_GetDebugLineBatcher(World, bPersistent, LifeTime, (DepthPriority == SDPG_Foreground)))
			{
				const float LineLifeTime = CF_GetDebugLineLifeTime(LineBatcher, LifeTime, bPersistent);

				FMatrix TransformMatrix;
				TransformMatrix.SetOrigin(Center);
				TransformMatrix.SetAxis(0,FVector(1,0,0));
				TransformMatrix.SetAxis(1,YAxis);
				TransformMatrix.SetAxis(2,ZAxis);
				
				// Need at least 4 segments
				NumSegments = FMath::Max(NumSegments, 4);
				CF_InternalDrawDebugCircle(World, TransformMatrix, Radius, NumSegments, LineColor.ToFColor(false), bPersistent, LifeTime, DepthPriority, Thickness);

				if (bDrawAxis)
				{
					const FVector Origin = TransformMatrix.GetOrigin();
					const FVector AxisY = TransformMatrix.GetScaledAxis( EAxis::Y );
					const FVector AxisZ = TransformMatrix.GetScaledAxis( EAxis::Z );

					TArray<FBatchedLine> Lines;
					Lines.Empty(2);
					Lines.Add(FBatchedLine(Origin - Radius * AxisY, Origin + Radius * AxisY, LineColor, LineLifeTime, Thickness, DepthPriority));
					Lines.Add(FBatchedLine(Origin - Radius * AxisZ, Origin + Radius * AxisZ, LineColor, LineLifeTime, Thickness, DepthPriority));
					LineBatcher->DrawLines(Lines);
				}
			}
		}
	}
}

void UKKHelperLibrary::Lib_DrawDebugPointSupportShipping(const UObject* WorldContextObject, const FVector Position,
	float Size, FLinearColor PointColor, bool bPersistent ,float LifeTime ,int32 DepthPriority)
{
	if (UWorld* InWorld = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		if (GEngine->GetNetMode(InWorld) != NM_DedicatedServer)
		{
			// this means foreground lines can't be persistent 
			if (ULineBatchComponent* const LineBatcher = CF_GetDebugLineBatcher(InWorld, bPersistent, LifeTime, (DepthPriority == SDPG_Foreground)))
			{
				const float PointLifeTime = CF_GetDebugLineLifeTime(LineBatcher, LifeTime, bPersistent);
				LineBatcher->DrawPoint(Position, PointColor, Size, DepthPriority, PointLifeTime);
			}
		}
	}
}

void UKKHelperLibrary::Lib_DrawDebugArrowSupportShipping(const UObject* WorldContextObject, const FVector LineStart,
	const FVector LineEnd, float ArrowSize, FLinearColor LineColor, bool bPersistent, float LifeTime,
	int32 DepthPriority,float Thickness)
{
	UWorld* InWorld = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (InWorld != nullptr)
	{
		if (GEngine->GetNetMode(InWorld) != NM_DedicatedServer)
		{
			if (ArrowSize <= 0)
			{
				ArrowSize = 10.f;
			}

			Lib_DrawDebugLineSupportShipping(InWorld, LineStart, LineEnd, LineColor, bPersistent, LifeTime, DepthPriority, Thickness);

			FVector Dir = (LineEnd-LineStart);
			Dir.Normalize();
			FVector Up(0, 0, 1);
			FVector Right = Dir ^ Up;
			if (!Right.IsNormalized())
			{
				Dir.FindBestAxisVectors(Up, Right);
			}
			FVector Origin = FVector::ZeroVector;
			FMatrix TM;
			// get matrix with dir/right/up
			TM.SetAxes(&Dir, &Right, &Up, &Origin);

			// since dir is x direction, my arrow will be pointing +y, -x and -y, -x
			float ArrowSqrt = FMath::Sqrt(ArrowSize);
			FVector ArrowPos;
			Lib_DrawDebugLineSupportShipping(InWorld, LineEnd, LineEnd + TM.TransformPosition(FVector(-ArrowSqrt, ArrowSqrt, 0)), LineColor, bPersistent, LifeTime, DepthPriority, Thickness);
			Lib_DrawDebugLineSupportShipping(InWorld, LineEnd, LineEnd + TM.TransformPosition(FVector(-ArrowSqrt, -ArrowSqrt, 0)), LineColor, bPersistent, LifeTime, DepthPriority, Thickness);
		}
	}
}

void UKKHelperLibrary::Lib_DrawDebugBoxSupportShipping(const UObject* WorldContextObject, const FVector Center,
	FVector Extent, FLinearColor LineColor, const FRotator Rotation, bool bPersistent, float LifeTime,
	int32 DepthPriority, float Thickness)
{
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		if (Rotation == FRotator::ZeroRotator)
		{
			CF_DrawDebugBox(World, Center, Extent, LineColor.ToFColor(true), bPersistent, LifeTime, SDPG_World, Thickness);
		}
		else
		{
			CF_DrawDebugBox(World, Center, Extent, Rotation.Quaternion(), LineColor.ToFColor(true), bPersistent, LifeTime, SDPG_World, Thickness);
		}
	}
}

void UKKHelperLibrary::Lib_DrawDebugCoordinateSystemSupportShipping(const UObject* WorldContextObject,
	const FVector AxisLoc, const FRotator AxisRot, float Scale, bool bPersistent, float LifeTime,
	int32 DepthPriority, float Thickness)
{
	if (UWorld* InWorld = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		if (GEngine->GetNetMode(InWorld) != NM_DedicatedServer)
		{
			FRotationMatrix R(AxisRot);
			FVector const X = R.GetScaledAxis( EAxis::X );
			FVector const Y = R.GetScaledAxis( EAxis::Y );
			FVector const Z = R.GetScaledAxis( EAxis::Z );

			// this means foreground lines can't be persistent 
			if (ULineBatchComponent* const LineBatcher = CF_GetDebugLineBatcher(InWorld, bPersistent, LifeTime, (DepthPriority == SDPG_Foreground)))
			{
				const float LineLifeTime = CF_GetDebugLineLifeTime(LineBatcher, LifeTime, bPersistent);
				LineBatcher->DrawLine(AxisLoc, AxisLoc + X*Scale, FColor::Red, DepthPriority, Thickness, LineLifeTime );
				LineBatcher->DrawLine(AxisLoc, AxisLoc + Y*Scale, FColor::Green, DepthPriority, Thickness, LineLifeTime );
				LineBatcher->DrawLine(AxisLoc, AxisLoc + Z*Scale, FColor::Blue, DepthPriority, Thickness, LineLifeTime );
			}
		}
	}
}

void UKKHelperLibrary::Lib_DrawDebugSphereSupportShipping(const UObject* WorldContextObject, const FVector Center,
	float Radius, int32 Segments, FLinearColor LineColor, bool bPersistent, float LifeTime, int32 DepthPriority,
	float Thickness)
{
	if (UWorld* InWorld = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		if (GEngine->GetNetMode(InWorld) != NM_DedicatedServer)
		{
			// this means foreground lines can't be persistent 
			if (ULineBatchComponent* const LineBatcher = CF_GetDebugLineBatcher(InWorld, bPersistent, LifeTime, (DepthPriority == SDPG_Foreground)))
			{
				float LineLifeTime = CF_GetDebugLineLifeTime(LineBatcher, LifeTime, bPersistent);

				// Need at least 4 segments
				Segments = FMath::Max(Segments, 4);

				FVector Vertex1, Vertex2, Vertex3, Vertex4;
				const float AngleInc = 2.f * PI / float(Segments);
				int32 NumSegmentsY = Segments;
				float Latitude = AngleInc;
				int32 NumSegmentsX;
				float Longitude;
				float SinY1 = 0.0f, CosY1 = 1.0f, SinY2, CosY2;
				float SinX, CosX;

				TArray<FBatchedLine> Lines;
				Lines.Empty(NumSegmentsY * Segments * 2);
				while (NumSegmentsY--)
				{
					SinY2 = FMath::Sin(Latitude);
					CosY2 = FMath::Cos(Latitude);

					Vertex1 = FVector(SinY1, 0.0f, CosY1) * Radius + Center;
					Vertex3 = FVector(SinY2, 0.0f, CosY2) * Radius + Center;
					Longitude = AngleInc;

					NumSegmentsX = Segments;
					while (NumSegmentsX--)
					{
						SinX = FMath::Sin(Longitude);
						CosX = FMath::Cos(Longitude);

						Vertex2 = FVector((CosX * SinY1), (SinX * SinY1), CosY1) * Radius + Center;
						Vertex4 = FVector((CosX * SinY2), (SinX * SinY2), CosY2) * Radius + Center;

						Lines.Add(FBatchedLine(Vertex1, Vertex2, LineColor, LineLifeTime, Thickness, DepthPriority));
						Lines.Add(FBatchedLine(Vertex1, Vertex3, LineColor, LineLifeTime, Thickness, DepthPriority));

						Vertex1 = Vertex2;
						Vertex3 = Vertex4;
						Longitude += AngleInc;
					}
					SinY1 = SinY2;
					CosY1 = CosY2;
					Latitude += AngleInc;
				}
				LineBatcher->DrawLines(Lines);
			}
		}
	}
}

void UKKHelperLibrary::Lib_DrawDebugCylinderSupportShipping(const UObject* WorldContextObject, const FVector Start,
	const FVector End, float Radius, int32 Segments, FLinearColor LineColor, bool bPersistent, float LifeTime,
	int32 DepthPriority, float Thickness)
{
	if (UWorld* InWorld = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		if (GEngine->GetNetMode(InWorld) != NM_DedicatedServer)
		{
			// this means foreground lines can't be persistent 
			if (ULineBatchComponent* const LineBatcher = CF_GetDebugLineBatcher(InWorld, bPersistent, LifeTime, (DepthPriority == SDPG_Foreground)))
			{
				// Need at least 4 segments
				Segments = FMath::Max(Segments, 4);

				// Rotate a point around axis to form cylinder segments
				FVector Segment;
				FVector P1, P2, P3, P4;
				const float AngleInc = 360.f / Segments;
				float Angle = AngleInc;

				// Default for Axis is up
				FVector Axis = (End - Start).GetSafeNormal();
				if( Axis.IsZero() )
				{
					Axis = FVector(0.f, 0.f, 1.f);
				}

				FVector Perpendicular;
				FVector Dummy;

				Axis.FindBestAxisVectors(Perpendicular, Dummy);
		
				Segment = Perpendicular.RotateAngleAxis(0, Axis) * Radius;
				P1 = Segment + Start;
				P3 = Segment + End;

				const float LineLifeTime = CF_GetDebugLineLifeTime(LineBatcher, LifeTime, bPersistent);
				while( Segments-- )
				{
					Segment = Perpendicular.RotateAngleAxis(Angle, Axis) * Radius;
					P2 = Segment + Start;
					P4 = Segment + End;

					LineBatcher->DrawLine(P2, P4, LineColor, DepthPriority, Thickness, LineLifeTime);
					LineBatcher->DrawLine(P1, P2, LineColor, DepthPriority, Thickness, LineLifeTime);
					LineBatcher->DrawLine(P3, P4, LineColor, DepthPriority, Thickness, LineLifeTime);

					P1 = P2;
					P3 = P4;
					Angle += AngleInc;
				}
			}
		}
	}
}

void UKKHelperLibrary::Lib_DrawDebugConeSupportShipping(UObject* WorldContextObject, const FVector Origin,
	const FVector Direction, float Length, float RadialWidth, float RadialHeight, int32 NumSides, FLinearColor LineColor,
	bool bPersistent, float LifeTime, int32 DepthPriority, float Thickness)
{
	if (GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		CF_DrawDebugCone(WorldContextObject, Origin, Direction, Length, RadialWidth, RadialHeight, NumSides, LineColor.ToFColor(true), bPersistent, LifeTime, DepthPriority, Thickness);
	}
}

void UKKHelperLibrary::Lib_DrawDebugConeInDegreesSupportShipping(UObject* WorldContextObject,
	const FVector Origin, const FVector Direction, float Length, float AngleWidth, float AngleHeight, int32 NumSides,
	FLinearColor LineColor, bool bPersistent, float LifeTime, int32 DepthPriority, float Thickness)
{
	if (GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		CF_DrawDebugCone(WorldContextObject, Origin, Direction, Length, FMath::DegreesToRadians(AngleWidth), FMath::DegreesToRadians(AngleHeight), NumSides, LineColor.ToFColor(true), bPersistent, LifeTime, DepthPriority, Thickness);
	}
}

void UKKHelperLibrary::Lib_DrawDebugCapsuleSupportShipping(const UObject* WorldContextObject, const FVector Center,
	float HalfHeight, float Radius, const FRotator Rotation, FLinearColor LineColor, bool bPersistent,
	float LifeTime, int32 DepthPriority, float Thickness)
{
	if (UWorld* InWorld = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		if (GEngine->GetNetMode(InWorld) != NM_DedicatedServer)
		{
			const int32 DrawCollisionSides = 16;

			FVector Origin = Center;
			FMatrix Axes = FQuatRotationTranslationMatrix(Rotation.Quaternion(), FVector::ZeroVector);
			FVector XAxis = Axes.GetScaledAxis( EAxis::X );
			FVector YAxis = Axes.GetScaledAxis( EAxis::Y );
			FVector ZAxis = Axes.GetScaledAxis( EAxis::Z ); 

			// Draw top and bottom circles
			float HalfAxis = FMath::Max<float>(HalfHeight - Radius, 1.f);
			FVector TopEnd = Origin + HalfAxis*ZAxis;
			FVector BottomEnd = Origin - HalfAxis*ZAxis;

			CF_DrawCircle(InWorld, TopEnd, XAxis, YAxis, LineColor, Radius, DrawCollisionSides, bPersistent, LifeTime, DepthPriority, Thickness);
			CF_DrawCircle(InWorld, BottomEnd, XAxis, YAxis, LineColor, Radius, DrawCollisionSides, bPersistent, LifeTime, DepthPriority, Thickness);

			// Draw domed caps
			CF_DrawHalfCircle(InWorld, TopEnd, YAxis, ZAxis, LineColor, Radius, DrawCollisionSides, bPersistent, LifeTime, DepthPriority, Thickness);
			CF_DrawHalfCircle(InWorld, TopEnd, XAxis, ZAxis, LineColor, Radius, DrawCollisionSides, bPersistent, LifeTime, DepthPriority, Thickness);

			FVector NegZAxis = -ZAxis;

			CF_DrawHalfCircle(InWorld, BottomEnd, YAxis, NegZAxis, LineColor, Radius, DrawCollisionSides, bPersistent, LifeTime, DepthPriority, Thickness);
			CF_DrawHalfCircle(InWorld, BottomEnd, XAxis, NegZAxis, LineColor, Radius, DrawCollisionSides, bPersistent, LifeTime, DepthPriority, Thickness);

			// Draw connected lines
			Lib_DrawDebugLineSupportShipping(InWorld, TopEnd + Radius*XAxis, BottomEnd + Radius*XAxis, LineColor, bPersistent, LifeTime, DepthPriority, Thickness);
			Lib_DrawDebugLineSupportShipping(InWorld, TopEnd - Radius*XAxis, BottomEnd - Radius*XAxis, LineColor, bPersistent, LifeTime, DepthPriority, Thickness);
			Lib_DrawDebugLineSupportShipping(InWorld, TopEnd + Radius*YAxis, BottomEnd + Radius*YAxis, LineColor, bPersistent, LifeTime, DepthPriority, Thickness);
			Lib_DrawDebugLineSupportShipping(InWorld, TopEnd - Radius*YAxis, BottomEnd - Radius*YAxis, LineColor, bPersistent, LifeTime, DepthPriority, Thickness);
		}
	}
}

void UKKHelperLibrary::Lib_DrawDebugStringSupportShipping(const UObject* WorldContextObject, const FVector TextLocation,
	const FString& Text, AActor* TestBaseActor, FLinearColor TextColor,float LifeTime,int32 FontScale, bool bDrawShadow)
{
	if (UWorld* InWorld = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		if (GEngine->GetNetMode(InWorld) != NM_DedicatedServer)
		{
			check((TestBaseActor == nullptr) || (TestBaseActor->GetWorld() == InWorld));
			AActor* BaseAct = (TestBaseActor != nullptr) ? TestBaseActor : InWorld->GetWorldSettings();

			// iterate through the player controller list
			for( FConstPlayerControllerIterator Iterator = InWorld->GetPlayerControllerIterator(); Iterator; ++Iterator )
			{
				APlayerController* PlayerController = Iterator->Get();
				if (PlayerController && PlayerController->MyHUD && PlayerController->Player)
				{
					PlayerController->MyHUD->AddDebugText(Text, BaseAct,LifeTime, TextLocation, TextLocation, TextColor.ToFColor(true), /*bSkipOverwriteCheck=*/ true, /*bAbsoluteLocation=*/ (TestBaseActor==nullptr), /*bKeepAttachedToActor=*/ false, nullptr, FontScale, bDrawShadow);
				}
			}
		}
	}
}

void UKKHelperLibrary::Lib_FlushDebugStringsSupportShipping(const UObject* WorldContextObject)
{
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		for( FConstPlayerControllerIterator Iterator = World->GetPlayerControllerIterator(); Iterator; ++Iterator )
		{
			// if it's a player
			APlayerController* PlayerController = Iterator->Get();
			if (PlayerController && PlayerController->MyHUD)
			{
				PlayerController->MyHUD->RemoveAllDebugStrings();
			}
		}	
	}
}

void UKKHelperLibrary::Lib_DrawDebugPlaneSupportShipping(const UObject* WorldContextObject,
	const FPlane& PlaneCoordinates, const FVector Location, float Size, FLinearColor PlaneColor, bool bPersistent,
	float LifeTime, int32 DepthPriority, float Thickness)
{
	if (UWorld* InWorld = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		if(GEngine->GetNetMode(InWorld) != NM_DedicatedServer)
		{
			FVector const ClosestPtOnPlane = Location - PlaneCoordinates.PlaneDot(Location) * PlaneCoordinates;

			FVector U, V;
			PlaneCoordinates.FindBestAxisVectors(U, V);
			U *= Size;
			V *= Size;

			TArray<FVector> Verts;
			Verts.AddUninitialized(4);
			Verts[0] = ClosestPtOnPlane + U + V;
			Verts[1] = ClosestPtOnPlane - U + V;
			Verts[2] = ClosestPtOnPlane + U - V;
			Verts[3] = ClosestPtOnPlane - U - V;

			TArray<int32> Indices;
			Indices.AddUninitialized(6);
			Indices[0] = 0; Indices[1] = 2; Indices[2] = 1;
			Indices[3] = 1; Indices[4] = 2; Indices[5] = 3;

			// plane quad
			CF_DrawDebugMesh(InWorld, Verts, Indices, PlaneColor.ToFColor(true), bPersistent, LifeTime, DepthPriority);

			// arrow indicating normal
			Lib_DrawDebugArrowSupportShipping(InWorld, ClosestPtOnPlane, ClosestPtOnPlane + PlaneCoordinates * 16.f, 8.f, FColor::White, bPersistent, LifeTime, DepthPriority,Thickness);
		}
	}
}

void UKKHelperLibrary::Lib_DrawDebugFrustumSupportShipping(UObject* WorldContextObject,
	const FTransform& FrustumTransform, FLinearColor FrustumColor, bool bPersistent, float LifeTime,
	int32 DepthPriority, float Thickness)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (World != nullptr && FrustumTransform.IsRotationNormalized())
	{
		FMatrix FrustumToWorld =  FrustumTransform.ToMatrixWithScale();
		CF_DrawDebugFrustum(WorldContextObject,FrustumToWorld,FrustumColor,bPersistent,LifeTime,DepthPriority,Thickness);
	}
}

void UKKHelperLibrary::Lib_DrawDebugCameraSupportShipping(const ACameraActor* CameraActor, FLinearColor CameraColor,
	bool bPersistent, float LifeTime, int32 DepthPriority, float Thickness)
{
	if(CameraActor)
	{
		CF_DrawDebugCamera(CameraActor->GetWorld(), CameraActor->GetActorLocation(), CameraActor->GetActorRotation(), CameraActor->GetCameraComponent()->FieldOfView, Thickness, CameraColor.ToFColor(true), bPersistent, LifeTime, SDPG_World);
	}
}

void UKKHelperLibrary::Lib_DrawDebugFloatHistoryTransformSupportShipping(const UObject* WorldContextObject,
	const FDebugFloatHistory& FloatHistory, const FTransform& DrawTransform, FVector2D DrawSize, FLinearColor DrawColor,
	bool bPersistent, float LifeTime, int32 DepthPriority)
{
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		CF_DrawDebugFloatHistory(*World, FloatHistory, DrawTransform, DrawSize, DrawColor.ToFColor(true), bPersistent, LifeTime,DepthPriority);
	}
}

void UKKHelperLibrary::Lib_DrawDebugFloatHistoryLocationSupportShipping(const UObject* WorldContextObject,
	const FDebugFloatHistory& FloatHistory, FVector DrawLocation, FVector2D DrawSize, FLinearColor DrawColor,
	bool bPersistent, float LifeTime, int32 DepthPriority)
{
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		CF_DrawDebugFloatHistory(*World, FloatHistory, DrawLocation, DrawSize, DrawColor.ToFColor(true), bPersistent, LifeTime,DepthPriority);
	}
}

FDebugFloatHistory UKKHelperLibrary::Lib_AddFloatHistorySample(float Value, const FDebugFloatHistory& FloatHistory)
{
	FDebugFloatHistory* const MutableFloatHistory = const_cast<FDebugFloatHistory*>(&FloatHistory);
	MutableFloatHistory->AddSample(Value);
	return FloatHistory;
}


void UKKHelperLibrary::Lib_FlushPersistentLinesSupportShipping(UObject* WorldContextObject)
{
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		if (World->PersistentLineBatcher)
		{
			World->PersistentLineBatcher->Flush();
		}
	}
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
		// 解析传递的 文件类型参数,比如  txt|*.txt|all files|*.*
		// 构建 COMDLG_FILTERSPEC 类型的数组,存储每种文件类型
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
		// 调用 SetFileTypes() 设置文件类型;
		FileDialog->SetFileTypes(FileDialogFilters.Num(), FileDialogFilters.GetData());

		// Show the picker
		if (SUCCEEDED(FileDialog->Show((HWND)ParentWindowHandle)))
		{
			OutFilterIndex = 0;
			// 获取当前选择的 文件类型索引, 就是之前设置的数组的Index : txt|*.txt|all files|*.*
			// 但是其默认索引是从1开始的,所以减去了1,获取了真实的索引值;
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

void UKKHelperLibrary::CF_InternalDrawDebugCircle(const UWorld* InWorld, const FMatrix& TransformMatrix, float Radius,
	int32 Segments, const FColor& Color, bool bPersistent, float LifeTime, uint8 DepthPriority, float Thickness)
{
	if (ULineBatchComponent* const LineBatcher = CF_GetDebugLineBatcher(InWorld, bPersistent, LifeTime, (DepthPriority == SDPG_Foreground)))
	{
		const float LineLifeTime = CF_GetDebugLineLifeTime(LineBatcher, LifeTime, bPersistent);

		// Need at least 4 segments
		Segments = FMath::Max(Segments, 4);
		const float AngleStep = 2.f * PI / float(Segments);

		const FVector Center = TransformMatrix.GetOrigin();
		const FVector AxisY = TransformMatrix.GetScaledAxis(EAxis::Y);
		const FVector AxisZ = TransformMatrix.GetScaledAxis(EAxis::Z);

		TArray<FBatchedLine> Lines;
		Lines.Empty(Segments);

		float Angle = 0.f;
		while (Segments--)
		{
			const FVector Vertex1 = Center + Radius * (AxisY * FMath::Cos(Angle) + AxisZ * FMath::Sin(Angle));
			Angle += AngleStep;
			const FVector Vertex2 = Center + Radius * (AxisY * FMath::Cos(Angle) + AxisZ * FMath::Sin(Angle));
			Lines.Add(FBatchedLine(Vertex1, Vertex2, Color, LineLifeTime, Thickness, DepthPriority));
		}
		LineBatcher->DrawLines(Lines);
	}
}

ULineBatchComponent* UKKHelperLibrary::CF_GetDebugLineBatcher(const UWorld* InWorld, bool bPersistent,
	float LifeTime, bool bDepthIsForeground)
{
	/**
		1.check world is valid ?
		2.check DepthPrrority == 1 , return World->ForegroundLineBatcher
		3.is not 1 ; check ( bPersistent || (LifeTime > 0.f) 
		4.is return PersistentLineBatcher | is not return LineBatcher
	*/
	return (InWorld ? (bDepthIsForeground ? InWorld->ForegroundLineBatcher : (( bPersistent || (LifeTime > 0.f) ) ? InWorld->PersistentLineBatcher : InWorld->LineBatcher)) : nullptr);
}

float UKKHelperLibrary::CF_GetDebugLineLifeTime(ULineBatchComponent* LineBatcher, float LifeTime, bool bPersistent)
{
	return bPersistent ? -1.0f : ((LifeTime > 0.f) ? LifeTime : LineBatcher->DefaultLifeTime);
}

void UKKHelperLibrary::CF_DrawDebugBox(const UWorld* InWorld, FVector const& Center, FVector const& Extent,
	FColor const& Color, bool bPersistent, float LifeTime, uint8 DepthPriority, float Thickness)
{
	// no debug line drawing on dedicated server
	if (GEngine->GetNetMode(InWorld) != NM_DedicatedServer)
	{
		// this means foreground lines can't be persistent 
		if (ULineBatchComponent* const LineBatcher = CF_GetDebugLineBatcher(InWorld, bPersistent, LifeTime, (DepthPriority == SDPG_Foreground)))
		{
			float LineLifeTime = CF_GetDebugLineLifeTime(LineBatcher, LifeTime, bPersistent);

			LineBatcher->DrawLine(Center + FVector( Extent.X,  Extent.Y,  Extent.Z), Center + FVector( Extent.X, -Extent.Y, Extent.Z), Color, DepthPriority, Thickness, LineLifeTime);
			LineBatcher->DrawLine(Center + FVector( Extent.X, -Extent.Y,  Extent.Z), Center + FVector(-Extent.X, -Extent.Y, Extent.Z), Color, DepthPriority, Thickness, LineLifeTime);
			LineBatcher->DrawLine(Center + FVector(-Extent.X, -Extent.Y,  Extent.Z), Center + FVector(-Extent.X,  Extent.Y, Extent.Z), Color, DepthPriority, Thickness, LineLifeTime);
			LineBatcher->DrawLine(Center + FVector(-Extent.X,  Extent.Y,  Extent.Z), Center + FVector( Extent.X,  Extent.Y, Extent.Z), Color, DepthPriority, Thickness, LineLifeTime);

			LineBatcher->DrawLine(Center + FVector( Extent.X,  Extent.Y, -Extent.Z), Center + FVector( Extent.X, -Extent.Y, -Extent.Z), Color, DepthPriority, Thickness, LineLifeTime);
			LineBatcher->DrawLine(Center + FVector( Extent.X, -Extent.Y, -Extent.Z), Center + FVector(-Extent.X, -Extent.Y, -Extent.Z), Color, DepthPriority, Thickness, LineLifeTime);
			LineBatcher->DrawLine(Center + FVector(-Extent.X, -Extent.Y, -Extent.Z), Center + FVector(-Extent.X,  Extent.Y, -Extent.Z), Color, DepthPriority, Thickness, LineLifeTime);
			LineBatcher->DrawLine(Center + FVector(-Extent.X,  Extent.Y, -Extent.Z), Center + FVector( Extent.X,  Extent.Y, -Extent.Z), Color, DepthPriority, Thickness, LineLifeTime);

			LineBatcher->DrawLine(Center + FVector( Extent.X,  Extent.Y,  Extent.Z), Center + FVector( Extent.X,  Extent.Y, -Extent.Z), Color, DepthPriority, Thickness, LineLifeTime);
			LineBatcher->DrawLine(Center + FVector( Extent.X, -Extent.Y,  Extent.Z), Center + FVector( Extent.X, -Extent.Y, -Extent.Z), Color, DepthPriority, Thickness, LineLifeTime);
			LineBatcher->DrawLine(Center + FVector(-Extent.X, -Extent.Y,  Extent.Z), Center + FVector(-Extent.X, -Extent.Y, -Extent.Z), Color, DepthPriority, Thickness, LineLifeTime);
			LineBatcher->DrawLine(Center + FVector(-Extent.X,  Extent.Y,  Extent.Z), Center + FVector(-Extent.X,  Extent.Y, -Extent.Z), Color, DepthPriority, Thickness, LineLifeTime);
		}
	}
}

void UKKHelperLibrary::CF_DrawDebugBox(const UWorld* InWorld, FVector const& Center, FVector const& Extent,
	const FQuat& Rotation, FColor const& Color, bool bPersistent, float LifeTime, uint8 DepthPriority,
	float Thickness)
{
	if (GEngine->GetNetMode(InWorld) != NM_DedicatedServer)
	{
		// this means foreground lines can't be persistent 
		if (ULineBatchComponent* const LineBatcher = CF_GetDebugLineBatcher(InWorld, bPersistent, LifeTime, (DepthPriority == SDPG_Foreground)))
		{
			float const LineLifeTime = CF_GetDebugLineLifeTime(LineBatcher, LifeTime, bPersistent);
			TArray<struct FBatchedLine> Lines;

			FTransform const Transform(Rotation);
			FVector Start = Transform.TransformPosition(FVector( Extent.X,  Extent.Y,  Extent.Z));
			FVector End = Transform.TransformPosition(FVector( Extent.X, -Extent.Y, Extent.Z));
			new(Lines) FBatchedLine(Center + Start, Center + End, Color, LineLifeTime, Thickness, DepthPriority);

			Start = Transform.TransformPosition(FVector( Extent.X, -Extent.Y,  Extent.Z));
			End = Transform.TransformPosition(FVector(-Extent.X, -Extent.Y, Extent.Z));
			new(Lines) FBatchedLine(Center + Start, Center + End, Color, LineLifeTime, Thickness, DepthPriority);

			Start = Transform.TransformPosition(FVector(-Extent.X, -Extent.Y,  Extent.Z));
			End = Transform.TransformPosition(FVector(-Extent.X,  Extent.Y, Extent.Z));
			new(Lines) FBatchedLine(Center + Start, Center + End, Color, LineLifeTime, Thickness, DepthPriority);

			Start = Transform.TransformPosition(FVector(-Extent.X,  Extent.Y,  Extent.Z));
			End = Transform.TransformPosition(FVector( Extent.X,  Extent.Y, Extent.Z));
			new(Lines) FBatchedLine(Center + Start, Center + End, Color, LineLifeTime, Thickness, DepthPriority);

			Start = Transform.TransformPosition(FVector( Extent.X,  Extent.Y, -Extent.Z));
			End = Transform.TransformPosition(FVector( Extent.X, -Extent.Y, -Extent.Z));
			new(Lines) FBatchedLine(Center + Start, Center + End, Color, LineLifeTime, Thickness, DepthPriority);

			Start = Transform.TransformPosition(FVector( Extent.X, -Extent.Y, -Extent.Z));
			End = Transform.TransformPosition(FVector(-Extent.X, -Extent.Y, -Extent.Z));
			new(Lines) FBatchedLine(Center + Start, Center + End, Color, LineLifeTime, Thickness, DepthPriority);

			Start = Transform.TransformPosition(FVector(-Extent.X, -Extent.Y, -Extent.Z));
			End = Transform.TransformPosition(FVector(-Extent.X,  Extent.Y, -Extent.Z));
			new(Lines) FBatchedLine(Center + Start, Center + End, Color, LineLifeTime, Thickness, DepthPriority);

			Start = Transform.TransformPosition(FVector(-Extent.X,  Extent.Y, -Extent.Z));
			End = Transform.TransformPosition(FVector( Extent.X,  Extent.Y, -Extent.Z));
			new(Lines )FBatchedLine(Center + Start, Center + End, Color, LineLifeTime, Thickness, DepthPriority);

			Start = Transform.TransformPosition(FVector( Extent.X,  Extent.Y,  Extent.Z));
			End = Transform.TransformPosition(FVector( Extent.X,  Extent.Y, -Extent.Z));
			new(Lines) FBatchedLine(Center + Start, Center + End, Color, LineLifeTime, Thickness, DepthPriority);

			Start = Transform.TransformPosition(FVector( Extent.X, -Extent.Y,  Extent.Z));
			End = Transform.TransformPosition(FVector( Extent.X, -Extent.Y, -Extent.Z));
			new(Lines) FBatchedLine(Center + Start, Center + End, Color, LineLifeTime, Thickness, DepthPriority);

			Start = Transform.TransformPosition(FVector(-Extent.X, -Extent.Y,  Extent.Z));
			End = Transform.TransformPosition(FVector(-Extent.X, -Extent.Y, -Extent.Z));
			new(Lines) FBatchedLine(Center + Start, Center + End, Color, LineLifeTime, Thickness, DepthPriority);

			Start = Transform.TransformPosition(FVector(-Extent.X,  Extent.Y,  Extent.Z));
			End = Transform.TransformPosition(FVector(-Extent.X,  Extent.Y, -Extent.Z));
			new(Lines) FBatchedLine(Center + Start, Center + End, Color, LineLifeTime, Thickness, DepthPriority);

			LineBatcher->DrawLines(Lines);
		}
	}
}

void UKKHelperLibrary::CF_DrawCircle( UObject* InWorldContext, const FVector& Base, const FVector& X, const FVector& Y,
	const FLinearColor& Color, float Radius, int32 NumSides, bool bPersistent, float LifeTime, uint8 DepthPriority,
	float Thickness)
{
	const float	AngleDelta = 2.0f * PI / NumSides;
	FVector	LastVertex = Base + X * Radius;

	for(int32 SideIndex = 0;SideIndex < NumSides;SideIndex++)
	{
		const FVector Vertex = Base + (X * FMath::Cos(AngleDelta * (SideIndex + 1)) + Y * FMath::Sin(AngleDelta * (SideIndex + 1))) * Radius;
		Lib_DrawDebugLineSupportShipping(InWorldContext, LastVertex, Vertex, Color, bPersistent, LifeTime, DepthPriority, Thickness);
		LastVertex = Vertex;
	}
}

void UKKHelperLibrary::CF_DrawHalfCircle(UObject* InWorldContext, const FVector& Base, const FVector& X,
	const FVector& Y, const FLinearColor& Color, float Radius, int32 NumSides, bool bPersistent, float LifeTime,
	uint8 DepthPriority, float Thickness)
{
	float	AngleDelta = 2.0f * (float)PI / ((float)NumSides);
	FVector	LastVertex = Base + X * Radius;

	for(int32 SideIndex = 0; SideIndex < (NumSides/2); SideIndex++)
	{
		FVector	Vertex = Base + (X * FMath::Cos(AngleDelta * (SideIndex + 1)) + Y * FMath::Sin(AngleDelta * (SideIndex + 1))) * Radius;
		Lib_DrawDebugLineSupportShipping(InWorldContext, LastVertex, Vertex, Color, bPersistent, LifeTime, DepthPriority, Thickness);
		LastVertex = Vertex;
	}	
}

void UKKHelperLibrary::CF_DrawDebugMesh(const UWorld* InWorld, TArray<FVector> const& Verts,
	TArray<int32> const& Indices, FColor const& Color, bool bPersistent, float LifeTime, uint8 DepthPriority)
{
	if (GEngine->GetNetMode(InWorld) != NM_DedicatedServer)
	{
		if (ULineBatchComponent* const LineBatcher = CF_GetDebugLineBatcher(InWorld, bPersistent, LifeTime, false))
		{
			float const ActualLifetime = CF_GetDebugLineLifeTime(LineBatcher, LifeTime, bPersistent);
			LineBatcher->DrawMesh(Verts, Indices, Color, DepthPriority, ActualLifetime);
		}
	}
}

void UKKHelperLibrary::CF_DrawDebugFrustum(UObject * InWorldContext, const FMatrix& FrustumToWorld, FLinearColor const& Color,
	bool bPersistentLines, float LifeTime, uint8 DepthPriority, float Thickness)
{
	if (GEngine->GetNetMode(InWorldContext->GetWorld()) != NM_DedicatedServer)
	{
		FVector Vertices[2][2][2];
		for(uint32 Z = 0;Z < 2;Z++)
		{
			for(uint32 Y = 0;Y < 2;Y++)
			{
				for(uint32 X = 0;X < 2;X++)
				{
					FVector4 UnprojectedVertex = FrustumToWorld.TransformFVector4(
						FVector4(
						(X ? -1.0f : 1.0f),
						(Y ? -1.0f : 1.0f),
						(Z ?  0.0f : 1.0f),
						1.0f
						)
						);
					Vertices[X][Y][Z] = FVector(UnprojectedVertex) / UnprojectedVertex.W;
				}
			}
		}

		Lib_DrawDebugLineSupportShipping(InWorldContext, Vertices[0][0][0], Vertices[0][0][1],Color,  bPersistentLines, LifeTime, DepthPriority, Thickness);
		Lib_DrawDebugLineSupportShipping(InWorldContext, Vertices[1][0][0], Vertices[1][0][1],Color,  bPersistentLines, LifeTime, DepthPriority, Thickness);
		Lib_DrawDebugLineSupportShipping(InWorldContext, Vertices[0][1][0], Vertices[0][1][1],Color,  bPersistentLines, LifeTime, DepthPriority, Thickness);
		Lib_DrawDebugLineSupportShipping(InWorldContext, Vertices[1][1][0], Vertices[1][1][1],Color,  bPersistentLines, LifeTime, DepthPriority, Thickness);

		Lib_DrawDebugLineSupportShipping(InWorldContext, Vertices[0][0][0], Vertices[0][1][0],Color,  bPersistentLines, LifeTime, DepthPriority, Thickness);
		Lib_DrawDebugLineSupportShipping(InWorldContext, Vertices[1][0][0], Vertices[1][1][0],Color,  bPersistentLines, LifeTime, DepthPriority, Thickness);
		Lib_DrawDebugLineSupportShipping(InWorldContext, Vertices[0][0][1], Vertices[0][1][1],Color,  bPersistentLines, LifeTime, DepthPriority, Thickness);
		Lib_DrawDebugLineSupportShipping(InWorldContext, Vertices[1][0][1], Vertices[1][1][1],Color,  bPersistentLines, LifeTime, DepthPriority, Thickness);

		Lib_DrawDebugLineSupportShipping(InWorldContext, Vertices[0][0][0], Vertices[1][0][0],Color,  bPersistentLines, LifeTime, DepthPriority, Thickness);
		Lib_DrawDebugLineSupportShipping(InWorldContext, Vertices[0][1][0], Vertices[1][1][0],Color,  bPersistentLines, LifeTime, DepthPriority, Thickness);
		Lib_DrawDebugLineSupportShipping(InWorldContext, Vertices[0][0][1], Vertices[1][0][1],Color,  bPersistentLines, LifeTime, DepthPriority, Thickness);
		Lib_DrawDebugLineSupportShipping(InWorldContext, Vertices[0][1][1], Vertices[1][1][1],Color,  bPersistentLines, LifeTime, DepthPriority, Thickness);
	}
}

void UKKHelperLibrary::CF_DrawDebugCamera(UObject* InWorldContext, FVector const& Location, FRotator const& Rotation,
	float FOVDeg, float Scale, FLinearColor const& Color, bool bPersistentLines, float LifeTime, uint8 DepthPriority)
{
	static float BaseScale = 4.f;
	static FVector BaseProportions(2.f, 1.f, 1.5f);
	UWorld * InWorld = InWorldContext->GetWorld();
	// no debug line drawing on dedicated server
	if (GEngine->GetNetMode(InWorld) != NM_DedicatedServer)
	{
		Lib_DrawDebugCoordinateSystemSupportShipping(InWorldContext, Location, Rotation, BaseScale*Scale, bPersistentLines, LifeTime, DepthPriority);
		FVector Extents = BaseProportions * BaseScale * Scale;
		CF_DrawDebugBox(InWorld, Location, Extents, Rotation.Quaternion(), Color.ToFColor(true), bPersistentLines, LifeTime, DepthPriority);		// lifetime

		// draw "lens" portion
		FRotationTranslationMatrix Axes(Rotation, Location);
		FVector XAxis = Axes.GetScaledAxis( EAxis::X );
		FVector YAxis = Axes.GetScaledAxis( EAxis::Y );
		FVector ZAxis = Axes.GetScaledAxis( EAxis::Z ); 

		FVector LensPoint = Location + XAxis * Extents.X;
		float LensSize = BaseProportions.Z * Scale * BaseScale;
		float HalfLensSize = LensSize * FMath::Tan(FMath::DegreesToRadians(FOVDeg*0.5f));
		FVector Corners[4] = 
		{
			LensPoint + XAxis * LensSize + (YAxis * HalfLensSize) + (ZAxis * HalfLensSize),
			LensPoint + XAxis * LensSize + (YAxis * HalfLensSize) - (ZAxis * HalfLensSize),
			LensPoint + XAxis * LensSize - (YAxis * HalfLensSize) - (ZAxis * HalfLensSize),
			LensPoint + XAxis * LensSize - (YAxis * HalfLensSize) + (ZAxis * HalfLensSize),
		};

		Lib_DrawDebugLineSupportShipping(InWorld, LensPoint, Corners[0], Color, bPersistentLines, LifeTime, DepthPriority);
		Lib_DrawDebugLineSupportShipping(InWorld, LensPoint, Corners[1], Color, bPersistentLines, LifeTime, DepthPriority);
		Lib_DrawDebugLineSupportShipping(InWorld, LensPoint, Corners[2], Color, bPersistentLines, LifeTime, DepthPriority);
		Lib_DrawDebugLineSupportShipping(InWorld, LensPoint, Corners[3], Color, bPersistentLines, LifeTime, DepthPriority);

		Lib_DrawDebugLineSupportShipping(InWorld, Corners[0], Corners[1], Color, bPersistentLines, LifeTime, DepthPriority);
		Lib_DrawDebugLineSupportShipping(InWorld, Corners[1], Corners[2], Color, bPersistentLines, LifeTime, DepthPriority);
		Lib_DrawDebugLineSupportShipping(InWorld, Corners[2], Corners[3], Color, bPersistentLines, LifeTime, DepthPriority);
		Lib_DrawDebugLineSupportShipping(InWorld, Corners[3], Corners[0], Color, bPersistentLines, LifeTime, DepthPriority);
	}
}

void UKKHelperLibrary::CF_DrawDebugFloatHistory(UWorld const& WorldRef, FDebugFloatHistory const& FloatHistory,
	FTransform const& DrawTransform, FVector2D const& DrawSize, FColor const& DrawColor, bool const& bPersistent,
	float const& LifeTime, uint8 const& DepthPriority)
{
	int const NumSamples = FloatHistory.GetNumSamples();
	if (NumSamples >= 2)
	{
		FVector DrawLocation = DrawTransform.GetLocation();
		FVector const AxisX = DrawTransform.GetUnitAxis(EAxis::Y);
		FVector const AxisY = DrawTransform.GetUnitAxis(EAxis::Z);
		FVector const AxisXStep = AxisX *  DrawSize.X / float(NumSamples);
		FVector const AxisYStep = AxisY *  DrawSize.Y / FMath::Max(FloatHistory.GetMinMaxRange(), KINDA_SMALL_NUMBER);

		AActor * WorldContext = WorldRef.GetFirstPlayerController();
		// Frame
		Lib_DrawDebugLineSupportShipping(WorldContext, DrawLocation, DrawLocation + AxisX * DrawSize.X, DrawColor, bPersistent, LifeTime, DepthPriority);
		Lib_DrawDebugLineSupportShipping(WorldContext, DrawLocation, DrawLocation + AxisY * DrawSize.Y, DrawColor, bPersistent, LifeTime, DepthPriority);
		Lib_DrawDebugLineSupportShipping(WorldContext, DrawLocation + AxisY * DrawSize.Y, DrawLocation + AxisX * DrawSize.X + AxisY * DrawSize.Y, DrawColor, bPersistent, LifeTime, DepthPriority);
		Lib_DrawDebugLineSupportShipping(WorldContext, DrawLocation + AxisX * DrawSize.X, DrawLocation + AxisX * DrawSize.X + AxisY * DrawSize.Y, DrawColor, bPersistent, LifeTime, DepthPriority);

		TArray<float> const & Samples = FloatHistory.GetSamples();

		TArray<FVector> Verts;
		Verts.AddUninitialized(NumSamples * 2);

		TArray<int32> Indices;
		Indices.AddUninitialized((NumSamples - 1) * 6);

		Verts[0] = DrawLocation;
		Verts[1] = DrawLocation + AxisYStep * Samples[0];

		for (int HistoryIndex = 1; HistoryIndex < NumSamples; HistoryIndex++)
		{
			DrawLocation += AxisXStep;

			int const VertIndex = (HistoryIndex - 1) * 2;
			Verts[VertIndex + 2] = DrawLocation;
			Verts[VertIndex + 3] = DrawLocation + AxisYStep * FMath::Clamp(Samples[HistoryIndex], FloatHistory.GetMinValue(), FloatHistory.GetMaxValue());

			int const StartIndex = (HistoryIndex - 1) * 6;
			Indices[StartIndex + 0] = VertIndex + 0; Indices[StartIndex + 1] = VertIndex + 1; Indices[StartIndex + 2] = VertIndex + 3;
			Indices[StartIndex + 3] = VertIndex + 0; Indices[StartIndex + 4] = VertIndex + 3; Indices[StartIndex + 5] = VertIndex + 2;
		}

		CF_DrawDebugMesh(&WorldRef, Verts, Indices, DrawColor, bPersistent, LifeTime, DepthPriority);
	}
}

void UKKHelperLibrary::CF_DrawDebugFloatHistory(UWorld const& WorldRef, FDebugFloatHistory const& FloatHistory,
	FVector const& DrawLocation, FVector2D const& DrawSize, FColor const& DrawColor, bool const& bPersistent,
	float const& LifeTime, uint8 const& DepthPriority)
{
	APlayerController * PlayerController = WorldRef.GetGameInstance() != nullptr ? WorldRef.GetGameInstance()->GetFirstLocalPlayerController() : nullptr;
	FRotator const DrawRotation = (PlayerController && PlayerController->PlayerCameraManager) ? PlayerController->PlayerCameraManager->GetCameraRotation() : FRotator(0, 0, 0);

	FTransform const DrawTransform(DrawRotation, DrawLocation);
	CF_DrawDebugFloatHistory(WorldRef, FloatHistory, DrawTransform, DrawSize, DrawColor, bPersistent, LifeTime, DepthPriority);
}

void UKKHelperLibrary::CF_DrawDebugCone(UObject* InWorldContext, FVector const& Origin, FVector const& Direction,
	float Length, float AngleWidth, float AngleHeight, int32 NumSides, FColor const& DrawColor, bool bPersistentLines,
	float LifeTime, uint8 DepthPriority, float Thickness)
{
	if (!InWorldContext)
	{
		return;
	}
	UWorld * InWorld = InWorldContext->GetWorld();
	if (!InWorld)
	{
		return;
	}
	// no debug line drawing on dedicated server
	if (GEngine->GetNetMode(InWorld) != NM_DedicatedServer)
	{
		// Need at least 4 sides
		NumSides = FMath::Max(NumSides, 4);

		const float Angle1 = FMath::Clamp<float>(AngleHeight, (float)KINDA_SMALL_NUMBER, (float)(PI - KINDA_SMALL_NUMBER));
		const float Angle2 = FMath::Clamp<float>(AngleWidth, (float)KINDA_SMALL_NUMBER, (float)(PI - KINDA_SMALL_NUMBER));

		const float SinX_2 = FMath::Sin(0.5f * Angle1);
		const float SinY_2 = FMath::Sin(0.5f * Angle2);

		const float SinSqX_2 = SinX_2 * SinX_2;
		const float SinSqY_2 = SinY_2 * SinY_2;

		const float TanX_2 = FMath::Tan(0.5f * Angle1);
		const float TanY_2 = FMath::Tan(0.5f * Angle2);

		TArray<FVector> ConeVerts;
		ConeVerts.AddUninitialized(NumSides);

		for(int32 i = 0; i < NumSides; i++)
		{
			const float Fraction	= (float)i/(float)(NumSides);
			const float Thi			= 2.f * PI * Fraction;
			const float Phi			= FMath::Atan2(FMath::Sin(Thi)*SinY_2, FMath::Cos(Thi)*SinX_2);
			const float SinPhi		= FMath::Sin(Phi);
			const float CosPhi		= FMath::Cos(Phi);
			const float SinSqPhi	= SinPhi*SinPhi;
			const float CosSqPhi	= CosPhi*CosPhi;

			const float RSq			= SinSqX_2*SinSqY_2 / (SinSqX_2*SinSqPhi + SinSqY_2*CosSqPhi);
			const float R			= FMath::Sqrt(RSq);
			const float Sqr			= FMath::Sqrt(1-RSq);
			const float Alpha		= R*CosPhi;
			const float Beta		= R*SinPhi;

			ConeVerts[i].X = (1 - 2*RSq);
			ConeVerts[i].Y = 2 * Sqr * Alpha;
			ConeVerts[i].Z = 2 * Sqr * Beta;
		}

		// Calculate transform for cone.
		FVector YAxis, ZAxis;
		FVector DirectionNorm = Direction.GetSafeNormal();
		DirectionNorm.FindBestAxisVectors(YAxis, ZAxis);
		const FMatrix ConeToWorld = FScaleMatrix(FVector(Length)) * FMatrix(DirectionNorm, YAxis, ZAxis, Origin);

		// this means foreground lines can't be persistent 
		if (ULineBatchComponent* const LineBatcher = CF_GetDebugLineBatcher(InWorld, bPersistentLines, LifeTime, (DepthPriority == SDPG_Foreground)))
		{
			float const LineLifeTime = CF_GetDebugLineLifeTime(LineBatcher, LifeTime, bPersistentLines);

			TArray<FBatchedLine> Lines;
			Lines.Empty(NumSides);

			FVector CurrentPoint, PrevPoint, FirstPoint;
			for(int32 i = 0; i < NumSides; i++)
			{
				CurrentPoint = ConeToWorld.TransformPosition(ConeVerts[i]);
				Lines.Add(FBatchedLine(ConeToWorld.GetOrigin(), CurrentPoint, DrawColor, LineLifeTime, Thickness, DepthPriority));

				// PrevPoint must be defined to draw junctions
				if( i > 0 )
				{
					Lines.Add(FBatchedLine(PrevPoint, CurrentPoint, DrawColor, LineLifeTime, Thickness, DepthPriority));
				}
				else
				{
					FirstPoint = CurrentPoint;
				}

				PrevPoint = CurrentPoint;
			}
			// Connect last junction to first
			Lines.Add(FBatchedLine(CurrentPoint, FirstPoint, DrawColor, LineLifeTime, Thickness, DepthPriority));

			LineBatcher->DrawLines(Lines);
		}
	}
}
