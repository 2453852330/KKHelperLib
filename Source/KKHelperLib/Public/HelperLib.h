// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "HelperLib.generated.h"

/**
 * 
 */
UCLASS()
class KKHELPERLIB_API UHelperLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/** 获取 Min - Max 范围内的随机点/向量 */
	UFUNCTION(BlueprintCallable,BlueprintPure,Category="KKHelperLib|Math")
	static FVector Lib_GetRandomPosInRandomRange(float MinRange = 0.f,float MaxRange = 1000.f);

	/** 获取绕某个轴向均匀发散的随机向量 */
	UFUNCTION(BlueprintCallable,BlueprintPure,Category="KKHelperLib|Math")
	static TArray<FVector> Lib_GetRandomDirAroundAxis(FVector InitDir,FVector AroundAxis,int32 Count);

	/** 修改Transform中的Scale */
	UFUNCTION(BlueprintCallable,BlueprintPure,Category="KKHelperLib|Transform")
	static FTransform & Lib_ChangeTransformScale(UPARAM(ref) FTransform & InTransform, FVector NewScale);



	/** Debug Info 在屏幕固定位置 */
	UFUNCTION(BlueprintCallable,Category="KKHelperLib|Debug",meta=(WorldContext=WorldContext))
	static void Lib_DebugStringOnFixedPos(const FString & InContent,int32 FixPos = 0 , float Time = 2.f, FColor Color = FColor::Red);
};
