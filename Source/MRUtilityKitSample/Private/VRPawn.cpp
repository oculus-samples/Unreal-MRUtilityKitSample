/*
Copyright (c) Meta Platforms, Inc. and affiliates.
All rights reserved.

This source code is licensed under the license found in the
LICENSE file in the root directory of this source tree.
*/

#include "VRPawn.h"
#include "EnhancedInputComponent.h"

AVRPawn::AVRPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	EnhancedInputComponent =
		CreateDefaultSubobject<UEnhancedInputComponent>("EnhancedInputComponent");
	InputComponent = EnhancedInputComponent;
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

void AVRPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AVRPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
