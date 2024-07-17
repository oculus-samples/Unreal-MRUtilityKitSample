/*
Copyright (c) Meta Platforms, Inc. and affiliates.
All rights reserved.

This source code is licensed under the license found in the
LICENSE file in the root directory of this source tree.
*/
#pragma once

#include "CoreMinimal.h"
#include "VRPawn.h"
#include "DemoVRPawn.generated.h"

UCLASS()
class ADemoVRPawn : public AVRPawn
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AActor> CubeActor;

	UPROPERTY(EditAnywhere)
	UMaterialInstance* CubeMaterial;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AActor> ArrowActor;

	UFUNCTION(BlueprintCallable)
	void HideShapes();

	UFUNCTION(BlueprintCallable)
	void DisplayCube(FVector Location, FRotator Rotation, FVector Scale = FVector(0.1), FVector Color = FVector(1.0));

	UFUNCTION(BlueprintCallable)
	void DisplayArrow(FVector Location, FVector Normal, int32 ArrowIndex = 0);

	UFUNCTION(BlueprintCallable)
	AActor* GetArrowSafe(int32 Index = 0);

protected:
	void BeginPlay() override;

private:
	UPROPERTY()
	TArray<AActor*> Arrows;

	UPROPERTY()
	AActor* Cube = nullptr;

	UPROPERTY()
	UMaterialInstanceDynamic* CubeMaterialInstance = nullptr;
};
