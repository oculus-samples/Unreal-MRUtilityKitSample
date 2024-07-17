/*
Copyright (c) Meta Platforms, Inc. and affiliates.
All rights reserved.

This source code is licensed under the license found in the
LICENSE file in the root directory of this source tree.
*/
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "MRUtilityKitRoom.h"
#include "DemoGameState.generated.h"

UCLASS()
class MRUTILITYKITSAMPLE_API ADemoGameState : public AGameState
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString CurrentDemoName = "Raycast";

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMRUKPositioningMethod BestPose = EMRUKPositioningMethod::Edge;
};
