/*
Copyright (c) Meta Platforms, Inc. and affiliates.
All rights reserved.

This source code is licensed under the license found in the
LICENSE file in the root directory of this source tree.
*/
#include "DemoVRPawn.h"
#include "DemoGameState.h"
#include "Kismet/KismetMathLibrary.h"

void ADemoVRPawn::BeginPlay()
{
	Super::BeginPlay();

	if (!Cube && IsValid(CubeActor))
	{
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		ActorSpawnParams.Owner = this;
		Cube = GetWorld()->SpawnActor<AActor>(CubeActor, ActorSpawnParams);

		Cube->SetActorScale3D(FVector(0.1));
		Cube->SetActorHiddenInGame(true);

		UStaticMeshComponent* StaticMesh = Cube->GetComponentByClass<UStaticMeshComponent>();
		check(StaticMesh);
		CubeMaterialInstance = UMaterialInstanceDynamic::Create(CubeMaterial, this);
		StaticMesh->SetMaterial(0, CubeMaterialInstance);
	}

	HideShapes();
}

void ADemoVRPawn::HideShapes()
{
	if (Cube)
	{
		Cube->SetActorHiddenInGame(true);
	}

	for (const auto Arrow : Arrows)
	{
		Arrow->SetActorHiddenInGame(true);
	}
}

void ADemoVRPawn::DisplayCube(FVector Location, FRotator Rotation, FVector Scale, FVector Color)
{
	CubeMaterialInstance->SetVectorParameterValue("ArrowColor", Color);

	Cube->SetActorLocation(Location);
	Cube->SetActorRotation(Rotation);
	Cube->SetActorScale3D(Scale);

	Cube->SetActorHiddenInGame(false);
}

void ADemoVRPawn::DisplayArrow(FVector Location, FVector Normal, int32 ArrowIndex)
{
	const auto Arrow = GetArrowSafe(ArrowIndex);
	Arrow->SetActorHiddenInGame(false);
	Arrow->SetActorLocation(Location);

	const bool IsNormalPointingUp = UKismetMathLibrary::NearlyEqual_FloatFloat(abs(Normal.Dot(FVector::UpVector)), 1.0, 0.01);

	FRotator Rotator;
	if (IsNormalPointingUp)
	{
		Rotator = UKismetMathLibrary::MakeRotFromZX(FVector::RightVector, Normal);
	}
	else
	{
		Rotator = UKismetMathLibrary::MakeRotFromZX(FVector::UpVector, Normal);
	}

	Arrow->SetActorRotation(Rotator);
}

AActor* ADemoVRPawn::GetArrowSafe(int32 Index)
{
	check(0 <= Index);
	check(ArrowActor);

	if (Index < Arrows.Num())
	{
		return Arrows[Index];
	}

	const int32 OldArrowsNum = Arrows.Num();
	Arrows.SetNum(Index + 1);

	for (int32 I = OldArrowsNum; I < Arrows.Num(); ++I)
	{
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		ActorSpawnParams.Owner = this;
		const auto Arrow = GetWorld()->SpawnActor<AActor>(ArrowActor, ActorSpawnParams);

		Arrow->SetActorScale3D(FVector(0.1));
		Arrow->SetActorHiddenInGame(true);

		Arrows[I] = Arrow;
	}

	return Arrows[Index];
}
