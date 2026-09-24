/*
Copyright (c) Meta Platforms, Inc. and affiliates.
All rights reserved.

This source code is licensed under the license found in the
LICENSE file in the root directory of this source tree.
*/

#include "DemoVRPawn.h"
#include "DemoGameState.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/KismetMathLibrary.h"
#include "IXRTrackingSystem.h"
#include "IHeadMountedDisplay.h"

static const FVector DefaultActorScale(0.1f);

ADemoVRPawn::ADemoVRPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	if (!IsValid(ShowMenuInputAction))
	{
		static ConstructorHelpers::FObjectFinder<UInputAction> ShowMenuAction(TEXT("/Game/Common/Input/Actions/IA_ShowMenu.IA_ShowMenu"));
		if (ShowMenuAction.Succeeded())
		{
			ShowMenuInputAction = ShowMenuAction.Object;
		}
	}
}

void ADemoVRPawn::BeginPlay()
{
	Super::BeginPlay();

	if (!Cube && IsValid(CubeActor))
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Owner = this;
		Cube = GetWorld()->SpawnActor<AActor>(CubeActor, SpawnParams);

		Cube->SetActorScale3D(DefaultActorScale);
		Cube->SetActorHiddenInGame(true);

		UStaticMeshComponent* StaticMesh = Cube->GetComponentByClass<UStaticMeshComponent>();
		check(StaticMesh);
		CubeMaterialInstance = UMaterialInstanceDynamic::Create(CubeMaterial, this);
		StaticMesh->SetMaterial(0, CubeMaterialInstance);
	}

	HideShapes();

	FTimerDelegate Timer;
	Timer.BindLambda([this]() {
		PlaceMenu();
	});
	GetWorldTimerManager().SetTimer(MenuDelayTimerHandle, Timer, 0.3f, false);
}

void ADemoVRPawn::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	for (AActor* Arrow : Arrows)
	{
		if (IsValid(Arrow))
		{
			Arrow->Destroy();
		}
	}
	Arrows.Empty();

	if (IsValid(Cube))
	{
		Cube->Destroy();
		Cube = nullptr;
	}

	CubeMaterialInstance = nullptr;

	Super::EndPlay(EndPlayReason);
}

void ADemoVRPawn::HideShapes()
{
	if (Cube)
	{
		Cube->SetActorHiddenInGame(true);
	}

	for (AActor* Arrow : Arrows)
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
	AActor* Arrow = GetArrowSafe(ArrowIndex);
	Arrow->SetActorHiddenInGame(false);
	Arrow->SetActorLocation(Location);
	const FRotator Rotator = UKismetMathLibrary::FindLookAtRotation(FVector::ZeroVector, Normal.GetSafeNormal());
	Arrow->SetActorRotation(Rotator);
}

AActor* ADemoVRPawn::GetArrowSafe(int32 Index)
{
	check(Index >= 0);
	check(ArrowActor);

	if (Index < Arrows.Num())
	{
		return Arrows[Index];
	}

	const int32 OldArrowsCount = Arrows.Num();
	Arrows.SetNum(Index + 1);

	for (int32 I = OldArrowsCount; I < Arrows.Num(); ++I)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Owner = this;
		AActor* const Arrow = GetWorld()->SpawnActor<AActor>(ArrowActor, SpawnParams);

		Arrow->SetActorScale3D(DefaultActorScale);
		Arrow->SetActorHiddenInGame(true);

		Arrows[I] = Arrow;
	}

	return Arrows[Index];
}

void ADemoVRPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInput->BindAction(ShowMenuInputAction, ETriggerEvent::Completed, this, &ADemoVRPawn::OnShowMenuCompleted);
	}
}

void ADemoVRPawn::OnShowMenuCompleted(const FInputActionValue& Value)
{
	PlaceMenu();
}

void ADemoVRPawn::PlaceMenu()
{
	if (!Menu)
	{
		return;
	}

	FQuat DeviceRotation;
	FVector DevicePosition;
	GEngine->XRSystem->GetCurrentPose(IXRTrackingSystem::HMDDeviceId, DeviceRotation, DevicePosition);

	FVector ForwardVector = DeviceRotation.GetForwardVector();
	FVector ForwardOffset = ForwardVector * MenuForwardDistance;

	FVector UpVector = DeviceRotation.GetUpVector();
	FVector VerticalOffsetVec = UpVector * MenuVerticalOffset;

	FVector NewLocation = DevicePosition + ForwardOffset + VerticalOffsetVec;

	Menu->SetActorLocationAndRotation(NewLocation, DeviceRotation, false, nullptr, ETeleportType::None);
}
