// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KKHelperLibType.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "KKHelperLibrary.generated.h"

/**
 * 
 */
UCLASS()
class KKHELPERLIB_API UKKHelperLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/** 获取 Min - Max 范围内的随机点/向量 */
	UFUNCTION(BlueprintCallable,BlueprintPure,Category="KKHelperLibrary|Math")
	static FVector Lib_GetRandomPosInRandomRange(float MinRange = 0.f,float MaxRange = 1000.f);
	/** 计算线段终点通过起点+朝向*长度 */
	UFUNCTION(BlueprintCallable,BlueprintPure,Category="KKHelperLibrary|Math")
	static FVector Lib_GetEndPointByStartAndDirection(FVector Start,FVector Dir,float Length);	
	/** 获取绕某个轴向均匀发散的随机向量 */
	UFUNCTION(BlueprintCallable,BlueprintPure,Category="KKHelperLibrary|Math")
	static TArray<FVector> Lib_GetRandomDirAroundAxis(FVector InitDir,FVector AroundAxis,int32 Count);

	/** 修改Transform中的Scale */
	UFUNCTION(BlueprintCallable,BlueprintPure,Category="KKHelperLibrary|Transform")
	static FTransform & Lib_ChangeTransformScale(UPARAM(ref) FTransform & InTransform, FVector NewScale);



	/** Debug Info 在屏幕固定位置 */
	UFUNCTION(BlueprintCallable,Category="KKHelperLibrary|Debug")
	static void Lib_DebugStringOnFixedPos(const FString & InContent,int32 FixPos = 0 , float Time = 2.f, FColor Color = FColor::Red);

	/** Debug Hit Result 信息 */
	UFUNCTION(BlueprintCallable,Category="KKHelperLibrary|Debug")
	static void Lib_DebugHitResult(const FHitResult & HitResult,bool bShowOnScreen = false,float Time = 2.f);

	/** 获取 NetMode 的字符串 */
	static FString Lib_GetNetModeString(ENetMode NetMode);
		
	/** 获取 Role 的字符串 */
	static FString Lib_GetRoleString(ENetRole NetRole);
	
	/** 获取 Actor Spawn 参数,总是生成 */
	static FActorSpawnParameters Lib_GetActorAlwaysSpawnParameter();


	/** 打开文件对话框1 */
	UFUNCTION(BlueprintCallable,Category="KKHelperLibrary|Dialog")
	static bool Lib_OpenFileDialog(const FString& DialogTitle, const FString& DefaultPath, const FString& DefaultFile, const FString& FileTypes, bool bMultiSelect, TArray<FString>& OutFilenames);
	/** 打开文件对话框2:包含FilterIndex: 假设有文件类型为 A|*.1|B|*.2|C|*.3|D|*.4|E|*.5 ,如果选中*.1文件,则为0 ,如果选中*.5文件,则为4;也就是数组的Index [*.1,*.2,*.3,*.4,*.5]  */
	UFUNCTION(BlueprintCallable,Category="KKHelperLibrary|Dialog")
	static bool Lib_OpenFileDialogWithIndex(const FString& DialogTitle, const FString& DefaultPath, const FString& DefaultFile, const FString& FileTypes, bool bMultiSelect, TArray<FString>& OutFilenames,int32& OutFilterIndex);
	/** 打开保存文件对话框,建议使用Simple版本 */
	UFUNCTION(BlueprintCallable,Category="KKHelperLibrary|Dialog")
	static bool Lib_SaveFileDialog(const FString& DialogTitle, const FString& DefaultPath, const FString& DefaultFile, const FString& FileTypes, bool bMultiSelect, TArray<FString>& OutFilenames);
	/** 打开保存文件对话框,参数少的模式 */
	UFUNCTION(BlueprintCallable,Category="KKHelperLibrary|Dialog")
	static bool Lib_SaveFileDialogSimple(const FString& DialogTitle, const FString& DefaultPath, const FString& FileTypes,FString & OutFilename);
	/** 打开目录选中对话框 */
	UFUNCTION(BlueprintCallable,Category="KKHelperLibrary|Dialog")
	static bool Lib_OpenDirectoryDialog(const FString& DialogTitle, const FString& DefaultPath, FString& OutFolderName);
	/** 打开字体对话框 */
	UFUNCTION(BlueprintCallable,Category="KKHelperLibrary|Dialog")
	static bool Lib_OpenFontDialog(FString& OutFontName, float& OutHeight, FFontImportFlagsBP & OutFlags);


	/** 绘制Debug线条,支持Shipping模式 */
	UFUNCTION(BlueprintCallable,Category="KKHelperLibrary|DrawDebug",meta=(WorldContext=WorldContextObject))
	static void Lib_DrawDebugLineSupportShipping(UObject * WorldContextObject,FVector LineStart,FVector LineEnd,FLinearColor LinearColor = FLinearColor::White,bool bPersistent = false ,float LifeTime = 1.f,int32 DepthPriority = 0,float Thickness = 0.f);
	/** 绘制圆环 */
	UFUNCTION(BlueprintCallable,Category="KKHelperLibrary|DrawDebug",meta=(WorldContext=WorldContextObject))
	static void Lib_DrawDebugCircleSupportShipping(const UObject* WorldContextObject, FVector Center, float Radius=50.f, int32 NumSegments=12, FLinearColor LineColor = FLinearColor::White,bool bPersistent = false, float LifeTime=0.f, int32 DepthPriority = 0,float Thickness=0.f, FVector YAxis=FVector(0.f,1.f,0.f),FVector ZAxis=FVector(0.f,0.f,1.f), bool bDrawAxis=false);
	/** 绘制点 */
	UFUNCTION(BlueprintCallable,Category="KKHelperLibrary|DrawDebug",meta=(WorldContext=WorldContextObject))
	static void Lib_DrawDebugPointSupportShipping(const UObject* WorldContextObject, const FVector Position, float Size = 3.f, FLinearColor PointColor = FLinearColor::White, bool bPersistent = false ,float LifeTime = 1.f,int32 DepthPriority = 0);
	/** 绘制箭头 */
	UFUNCTION(BlueprintCallable,Category="KKHelperLibrary|DrawDebug",meta=(WorldContext=WorldContextObject))
	static void Lib_DrawDebugArrowSupportShipping(const UObject* WorldContextObject, const FVector LineStart, const FVector LineEnd, float ArrowSize = 20.f, FLinearColor LineColor = FLinearColor::White, bool bPersistent = false ,float LifeTime = 1.f,int32 DepthPriority = 0,float Thickness = 0.f);
	/** 绘制方框 */
	UFUNCTION(BlueprintCallable,Category="KKHelperLibrary|DrawDebug",meta=(WorldContext=WorldContextObject))
	static void Lib_DrawDebugBoxSupportShipping(const UObject* WorldContextObject, const FVector Center, FVector Extent, FLinearColor LineColor = FLinearColor::White , const FRotator Rotation = FRotator::ZeroRotator, bool bPersistent = false ,float LifeTime = 1.f,int32 DepthPriority = 0, float Thickness = 0.f);
	/** 绘制坐标轴 */
	UFUNCTION(BlueprintCallable,Category="KKHelperLibrary|DrawDebug",meta=(WorldContext=WorldContextObject))
	static void Lib_DrawDebugCoordinateSystemSupportShipping(const UObject* WorldContextObject, const FVector AxisLoc, const FRotator AxisRot, float Scale = 1.f,bool bPersistent = false ,float LifeTime = 1.f,int32 DepthPriority = 0,float Thickness = 0.f);
	/** 绘制球 */
	UFUNCTION(BlueprintCallable,Category="KKHelperLibrary|DrawDebug",meta=(WorldContext=WorldContextObject))
	static void Lib_DrawDebugSphereSupportShipping(const UObject* WorldContextObject, const FVector Center, float Radius = 100.f, int32 Segments = 12, FLinearColor LineColor = FLinearColor::White, bool bPersistent = false ,float LifeTime = 1.f,int32 DepthPriority = 0,float Thickness = 0.f);
	/** 绘制圆柱 */
	UFUNCTION(BlueprintCallable,Category="KKHelperLibrary|DrawDebug",meta=(WorldContext=WorldContextObject))
	static void Lib_DrawDebugCylinderSupportShipping(const UObject* WorldContextObject, const FVector Start, const FVector End, float Radius = 100.f, int32 Segments = 12, FLinearColor LineColor = FLinearColor::White, bool bPersistent = false ,float LifeTime = 1.f,int32 DepthPriority = 0,float Thickness = 0.f);
	/** 绘制圆锥-弧度版本 */
	UFUNCTION(BlueprintCallable,Category="KKHelperLibrary|DrawDebug",meta=(WorldContext=WorldContextObject))
	static void Lib_DrawDebugConeSupportShipping(UObject* WorldContextObject, const FVector Origin, const FVector Direction = FVector(1.f,0.f,0.f), float Length = 100.f, float RadialWidth = 0.5236f , float RadialHeight = 0.5236f , int32 NumSides = 12, FLinearColor LineColor = FLinearColor::White,  bool bPersistent = false ,float LifeTime = 1.f,int32 DepthPriority = 0,float Thickness = 0.f);
	/** 绘制圆锥-角度版本 */
	UFUNCTION(BlueprintCallable,Category="KKHelperLibrary|DrawDebug",meta=(WorldContext=WorldContextObject))
	static void Lib_DrawDebugConeInDegreesSupportShipping(UObject* WorldContextObject, const FVector Origin, const FVector Direction = FVector(1.f,0.f,0.f), float Length=100.f, float AngleWidth = 45.f, float AngleHeight = 45.f, int32 NumSides = 12, FLinearColor LineColor = FLinearColor::White,  bool bPersistent = false ,float LifeTime = 1.f,int32 DepthPriority = 0,float Thickness = 0.f);
	/** 绘制胶囊体 */
	UFUNCTION(BlueprintCallable,Category="KKHelperLibrary|DrawDebug",meta=(WorldContext=WorldContextObject))
	static void Lib_DrawDebugCapsuleSupportShipping(const UObject* WorldContextObject, const FVector Center, float HalfHeight = 44.f , float Radius = 30.f, const FRotator Rotation = FRotator::ZeroRotator, FLinearColor LineColor = FLinearColor::White,  bool bPersistent = false ,float LifeTime = 1.f,int32 DepthPriority = 0,float Thickness = 0.f);
	/** 绘制文字 */
	UFUNCTION(BlueprintCallable,Category="KKHelperLibrary|DrawDebug",meta=(WorldContext=WorldContextObject))
	static void Lib_DrawDebugStringSupportShipping(const UObject* WorldContextObject, const FVector TextLocation, const FString& Text, class AActor* TestBaseActor = NULL, FLinearColor TextColor = FLinearColor::White,  float LifeTime = 1.f,int32 FontScale = 1 , bool bDrawShadow = false);
	/** 清除绘制的文字(只清除DrawDebugString) */
	UFUNCTION(BlueprintCallable,Category="KKHelperLibrary|DrawDebug",meta=(WorldContext=WorldContextObject))
	static void Lib_FlushDebugStringsSupportShipping(const UObject* WorldContextObject);
	/** 绘制平面 */
	UFUNCTION(BlueprintCallable,Category="KKHelperLibrary|DrawDebug",meta=(WorldContext=WorldContextObject))
	static void Lib_DrawDebugPlaneSupportShipping(const UObject* WorldContextObject, const FPlane& PlaneCoordinates, const FVector Location, float Size, FLinearColor PlaneColor = FLinearColor::White,  bool bPersistent = false ,float LifeTime = 1.f,int32 DepthPriority = 0,float Thickness = 0.f);
	/** 绘制视锥体 */
	UFUNCTION(BlueprintCallable,Category="KKHelperLibrary|DrawDebug",meta=(WorldContext=WorldContextObject))
	static void Lib_DrawDebugFrustumSupportShipping(UObject* WorldContextObject, const FTransform& FrustumTransform, FLinearColor FrustumColor = FLinearColor::White, bool bPersistent = false ,float LifeTime = 1.f,int32 DepthPriority = 0,float Thickness = 0.f);
	/** 绘制相机Actor */
	UFUNCTION(BlueprintCallable,Category="KKHelperLibrary|DrawDebug",meta=(WorldContext=WorldContextObject))
	static void Lib_DrawDebugCameraSupportShipping(const ACameraActor* CameraActor, FLinearColor CameraColor = FLinearColor::White, bool bPersistent = false ,float LifeTime = 1.f,int32 DepthPriority = 0,float Thickness = 0.f);
	/** 绘制float数据的直方图-通过Transform手动传递朝向 */
	UFUNCTION(BlueprintCallable,Category="KKHelperLibrary|DrawDebug",meta=(WorldContext=WorldContextObject))
	static void Lib_DrawDebugFloatHistoryTransformSupportShipping(const UObject* WorldContextObject, const FDebugFloatHistory& FloatHistory, const FTransform& DrawTransform, FVector2D DrawSize, FLinearColor DrawColor = FLinearColor::White, bool bPersistent = false ,float LifeTime = 1.f,int32 DepthPriority = 0);
	/** 绘制float数据的直方图-自动调整朝向 */
	UFUNCTION(BlueprintCallable,Category="KKHelperLibrary|DrawDebug",meta=(WorldContext=WorldContextObject))
	static void Lib_DrawDebugFloatHistoryLocationSupportShipping(const UObject* WorldContextObject, const FDebugFloatHistory& FloatHistory, FVector DrawLocation, FVector2D DrawSize, FLinearColor DrawColor = FLinearColor::White, bool bPersistent = false ,float LifeTime = 1.f,int32 DepthPriority = 0);
	/** 给绘制float的直方图添加点位数据 */
	UFUNCTION(BlueprintCallable,BlueprintPure,Category="KKHelperLibrary|DrawDebug",meta=(WorldContext=WorldContextObject))
	static FDebugFloatHistory Lib_AddFloatHistorySample(float Value, const FDebugFloatHistory& FloatHistory);
	/** 清除绘制的Debug信息,支持Shipping模式 */
	UFUNCTION(BlueprintCallable,Category="KKHelperLibrary|DrawDebug",meta=(WorldContext=WorldContextObject))
	static void Lib_FlushPersistentLinesSupportShipping(UObject * WorldContextObject);
private:
	// 打开对话框的实现函数
	static bool CF_FileDialogShared(bool bSave, const void* ParentWindowHandle, const FString& DialogTitle, const FString& DefaultPath, const FString& DefaultFile, const FString& FileTypes, uint32 Flags, TArray<FString>& OutFilenames, int32& OutFilterIndex);
	// 绘制函数的功能实现
	static void CF_InternalDrawDebugCircle(const UWorld* InWorld, const FMatrix& TransformMatrix, float Radius, int32 Segments, const FColor& Color, bool bPersistent, float LifeTime, uint8 DepthPriority, float Thickness = 0.f);
	static ULineBatchComponent* CF_GetDebugLineBatcher( const UWorld* InWorld, bool bPersistent, float LifeTime, bool bDepthIsForeground );
	static float CF_GetDebugLineLifeTime(ULineBatchComponent* LineBatcher, float LifeTime, bool bPersistent);
	static void CF_DrawDebugBox(const UWorld* InWorld, FVector const& Center, FVector const& Extent, FColor const& Color, bool bPersistent = false, float LifeTime=-1.f, uint8 DepthPriority = 0, float Thickness = 0.f);
	static void CF_DrawDebugBox(const UWorld* InWorld, FVector const& Center, FVector const& Extent, const FQuat& Rotation, FColor const& Color, bool bPersistent = false, float LifeTime=-1.f, uint8 DepthPriority = 0, float Thickness = 0.f);
	static void CF_DrawCircle( UObject* InWorldContext, const FVector& Base, const FVector& X, const FVector& Y, const FLinearColor& Color, float Radius, int32 NumSides, bool bPersistent = false, float LifeTime = -1.f, uint8 DepthPriority = 0, float Thickness = 0);
	static void CF_DrawHalfCircle(UObject * InWorldContext, const FVector& Base, const FVector& X, const FVector& Y, const FLinearColor& Color, float Radius, int32 NumSides, bool bPersistent, float LifeTime, uint8 DepthPriority, float Thickness);
	static void CF_DrawDebugMesh(const UWorld* InWorld, TArray<FVector> const& Verts, TArray<int32> const& Indices, FColor const& Color, bool bPersistent, float LifeTime, uint8 DepthPriority);
	static void CF_DrawDebugFrustum(UObject * InWorldContext, const FMatrix& FrustumToWorld, FLinearColor const& Color, bool bPersistentLines = false, float LifeTime=-1.f, uint8 DepthPriority = 0, float Thickness = 0.f);
	static void CF_DrawDebugCamera(UObject * InWorldContext, FVector const& Location, FRotator const& Rotation, float FOVDeg, float Scale, FLinearColor const& Color, bool bPersistentLines, float LifeTime, uint8 DepthPriority);
	static void CF_DrawDebugFloatHistory(UWorld const & WorldRef, FDebugFloatHistory const & FloatHistory, FTransform const & DrawTransform, FVector2D const & DrawSize, FColor const & DrawColor, bool const & bPersistent, float const & LifeTime, uint8 const & DepthPriority);
	static void CF_DrawDebugFloatHistory(UWorld const & WorldRef, FDebugFloatHistory const & FloatHistory, FVector const & DrawLocation, FVector2D const & DrawSize, FColor const & DrawColor, bool const & bPersistent, float const & LifeTime, uint8 const & DepthPriority);
	static void CF_DrawDebugCone(UObject * InWorldContext, FVector const& Origin, FVector const& Direction, float Length, float AngleWidth, float AngleHeight, int32 NumSides, FColor const& DrawColor, bool bPersistentLines, float LifeTime, uint8 DepthPriority, float Thickness);
	//
	

};
