/*
Copyright (c) Meta Platforms, Inc. and affiliates.
All rights reserved.

This source code is licensed under the license found in the
LICENSE file in the root directory of this source tree.
*/
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "OculusXRLegacyPoseTransformComponent.h"
#include "VRPawn.generated.h"

UCLASS()
class AVRPawn : public APawn
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USceneComponent* Root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UMotionControllerComponent* MotionControllerLeft;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UMotionControllerComponent* MotionControllerRight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UOculusXRLegacyPoseTransformComponent* LegacyPoseTransformLeft;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UOculusXRLegacyPoseTransformComponent* LegacyPoseTransformRight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UXRDeviceVisualizationComponent* DeviceVisualizationLeft;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UXRDeviceVisualizationComponent* DeviceVisualizationRight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UWidgetComponent* DemoMenu;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UWidgetInteractionComponent* WidgetInteraction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USplineComponent* Spline;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USplineMeshComponent* SplineMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TSoftObjectPtr<class UInputMappingContext> InputMapping;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* MenuAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* ClickAction;

public:
	AVRPawn();

	void BeginPlay() override;
	void Tick(float DeltaTime) override;

	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	void InputMenu(const FInputActionValue& Value);
	void InputClick(const FInputActionValue& Value);
	void InputRelease(const FInputActionValue& Value);

private:
	void UpdateWidgetBeam() const;
	void ShowHideControllers() const;
};
