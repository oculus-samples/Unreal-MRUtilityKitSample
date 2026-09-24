/*
Copyright (c) Meta Platforms, Inc. and affiliates.
All rights reserved.

This source code is licensed under the license found in the
LICENSE file in the root directory of this source tree.
*/
#pragma once

#include "CoreMinimal.h"
#include "VRPawn.h"
#include "InputActionValue.h"
#include "DemoVRPawn.generated.h"

class UInputAction;

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* ShowMenuInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu")
	AActor* Menu;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu")
	float MenuForwardDistance = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu")
	float MenuVerticalOffset = -20.0f;

	UFUNCTION(BlueprintCallable)
	void HideShapes();

	UFUNCTION(BlueprintCallable)
	void DisplayCube(FVector Location, FRotator Rotation, FVector Scale = FVector(0.1), FVector Color = FVector(1.0));

	UFUNCTION(BlueprintCallable)
	void DisplayArrow(FVector Location, FVector Normal, int32 ArrowIndex = 0);

	UFUNCTION(BlueprintCallable)
	AActor* GetArrowSafe(int32 Index = 0);

	ADemoVRPawn();

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:
	void BeginPlay() override;
	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UPROPERTY()
	TArray<AActor*> Arrows;

	UPROPERTY()
	AActor* Cube = nullptr;

	UPROPERTY()
	UMaterialInstanceDynamic* CubeMaterialInstance = nullptr;

	FTimerHandle MenuDelayTimerHandle;

	void OnShowMenuCompleted(const FInputActionValue& Value);
	void PlaceMenu();
};
