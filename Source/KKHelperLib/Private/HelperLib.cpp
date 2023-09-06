// Fill out your copyright notice in the Description page of Project Settings.


#include "HelperLib.h"

#include "PhysicalMaterials/PhysicalMaterial.h"


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

void UHelperLib::Lib_DebugHitResult(const FHitResult& HitResult, bool bShowOnScreen, float Time)
{
	FString DebugString = FString::Printf(TEXT("BlockingHit:%d | InitialOverlap:%d | Time:%f | Distance:%f | Location:%s | ImpactPoint:%s | Normal:%s | ImpactNormal:%s | PhysMatName:%s | HitActorName:%s | HitComponent:%s | HitBoneName:%s | HitItem:%d | FaceIndex:%d | TraceStart:%s | TraceEnd:%s"),
		HitResult.bBlockingHit,HitResult.bStartPenetrating,HitResult.Time,HitResult.Distance,*HitResult.Location.ToString(),*HitResult.ImpactPoint.ToString(),*HitResult.Normal.ToString(),
		*HitResult.ImpactNormal.ToString(),HitResult.PhysMaterial.IsValid()?*HitResult.PhysMaterial.Get()->GetName():TEXT("NULL"),
		HitResult.GetActor()?*HitResult.GetActor()->GetName():TEXT("NULL"),HitResult.GetComponent()?*HitResult.GetComponent()->GetName():TEXT("NULL"),
		*HitResult.BoneName.ToString(),HitResult.Item,HitResult.FaceIndex,*HitResult.TraceStart.ToString(),*HitResult.TraceEnd.ToString());

	if (bShowOnScreen && GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1,Time,FColor::Red,DebugString);
	}
	UE_LOG(LogTemp,Warning,TEXT("[HelperLib:51]HitResult:%s"),*DebugString);
}

FString UHelperLib::Lib_GetNetModeString(ENetMode NetMode)
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

FString UHelperLib::Lib_GetRoleString(ENetRole NetRole)
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

FActorSpawnParameters UHelperLib::Lib_GetActorAlwaysSpawnParameter_C()
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	return SpawnParameters;
}

