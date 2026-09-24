/*
Copyright (c) Meta Platforms, Inc. and affiliates.
All rights reserved.

This source code is licensed under the license found in the
LICENSE file in the root directory of this source tree.
*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "VRPawn.generated.h"

UCLASS()
class AVRPawn : public APawn
{
	GENERATED_BODY()

public:
	AVRPawn();

	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UEnhancedInputComponent> EnhancedInputComponent;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
};
