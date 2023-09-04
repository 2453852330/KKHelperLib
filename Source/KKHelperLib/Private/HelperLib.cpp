// Fill out your copyright notice in the Description page of Project Settings.


#include "HelperLib.h"


FVector UHelperLib::Lib_GetRandomPosInRandomRange(float MinRange, float MaxRange)
{
	return FMath::VRand() * FMath::RandRange(MinRange,MaxRange);
}

TArray<FVector> UHelperLib::Lib_GetRandomDirAroundAxis(FVector InitDir, FVector AroundAxis, int32 Count)
{
	TArray<FVector> List;
	float PerAngle = 360.f / Count;
	for (int32 i = 0; i < Count; ++i)
	{
		List.Emplace(InitDir.RotateAngleAxis(PerAngle * i , AroundAxis));
	}
	return List;
}

FTransform& UHelperLib::Lib_ChangeTransformScale(FTransform& InTransform, FVector NewScale)
{
	InTransform.SetScale3D(NewScale);
	return InTransform; 
}

void UHelperLib::Lib_DebugStringOnFixedPos(const FString& InContent, int32 FixPos, float Time, FColor Color)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(FixPos,Time,Color,InContent);
	}
}

