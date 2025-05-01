/*
Copyright (c) Meta Platforms, Inc. and affiliates.
All rights reserved.

This source code is licensed under the license found in the
LICENSE file in the root directory of this source tree.
*/
#include "VRPawn.h"
#include "OculusXRFunctionLibrary.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "Camera/CameraComponent.h"
#include "MotionControllerComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/WidgetInteractionComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "XRDeviceVisualizationComponent.h"

AVRPawn::AVRPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create components
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	MotionControllerLeft = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionControllerLeft"));
	MotionControllerRight = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionControllerRight"));
	LegacyPoseTransformLeft = CreateDefaultSubobject<UOculusXRLegacyPoseTransformComponent>(TEXT("LegacyPoseTransformLeft"));
	LegacyPoseTransformRight = CreateDefaultSubobject<UOculusXRLegacyPoseTransformComponent>(TEXT("LegacyPoseTransformRight"));
	DeviceVisualizationLeft = CreateDefaultSubobject<UXRDeviceVisualizationComponent>(TEXT("DeviceVisualizationLeft"));
	DeviceVisualizationRight = CreateDefaultSubobject<UXRDeviceVisualizationComponent>(TEXT("DeviceVisualizationRight"));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	DemoMenu = CreateDefaultSubobject<UWidgetComponent>(TEXT("DemoMenu"));
	WidgetInteraction = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("WidgetInteraction"));
	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	SplineMesh = CreateDefaultSubobject<USplineMeshComponent>(TEXT("SplineMesh"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	RootComponent = Root;

	// Setup components

	MotionControllerLeft->MotionSource = TEXT("Left");
	MotionControllerLeft->SetupAttachment(Root);

	MotionControllerRight->MotionSource = TEXT("Right");
	MotionControllerRight->SetupAttachment(Root);

	LegacyPoseTransformLeft->SetupAttachment(MotionControllerLeft);
	LegacyPoseTransformRight->SetupAttachment(MotionControllerRight);

	SpringArm->SetupAttachment(Root);
	Camera->SetupAttachment(Root);

	DeviceVisualizationLeft->SetIsVisualizationActive(true);
	DeviceVisualizationLeft->SetupAttachment(LegacyPoseTransformLeft);

	DeviceVisualizationRight->SetIsVisualizationActive(true);
	DeviceVisualizationRight->SetupAttachment(LegacyPoseTransformRight);

	DemoMenu->SetupAttachment(SpringArm);
	DemoMenu->SetRelativeRotation(FRotator::MakeFromEuler(FVector(0.0, 0.0, 180.0)));

	SpringArm->SetupAttachment(LegacyPoseTransformLeft);
	SpringArm->TargetArmLength = 0.0;
	SpringArm->bInheritYaw = true;
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritRoll = true;

	WidgetInteraction->SetupAttachment(LegacyPoseTransformRight);

	Spline->SetupAttachment(LegacyPoseTransformRight);

	SplineMesh->SetupAttachment(Spline);
	SplineMesh->SetMobility(EComponentMobility::Movable);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BeamMeshFinder(TEXT("/Game/Common/Models/SM_BeamMesh"));
	if (BeamMeshFinder.Succeeded())
	{
		SplineMesh->SetStaticMesh(BeamMeshFinder.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> BeamMaterialFinder(TEXT("/Game/Common/Materials/M_Beam"));
	if (BeamMaterialFinder.Succeeded())
	{
		SplineMesh->SetMaterial(0, BeamMaterialFinder.Object);
	}
	SplineMesh->SetStartPosition(FVector::ZeroVector);
	SplineMesh->SetStartTangent(FVector(100.0, 0.0, 0.0));
	SplineMesh->SetEndPosition(FVector(100.0, 0.0, 0.0));
	SplineMesh->SetEndTangent(FVector(100.0, 0.0, 0.0));

	// Load default input context
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContextFinder(TEXT("/Game/Common/Input/IMC_Default"));
	if (InputMappingContextFinder.Succeeded())
	{
		InputMapping = InputMappingContextFinder.Object;
	}

	// Load default input actions

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionSelectFinder(TEXT("/Game/Common/Input/Actions/IA_Select"));
	if (InputActionSelectFinder.Succeeded())
	{
		ClickAction = InputActionSelectFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShowMenuFinder(TEXT("/Game/Common/Input/Actions/IA_ShowMenu"));
	if (InputActionShowMenuFinder.Succeeded())
	{
		MenuAction = InputActionShowMenuFinder.Object;
	}
}

void AVRPawn::BeginPlay()
{
	Super::BeginPlay();

	if (!InputMapping.IsNull())
	{
		if (const auto PlayerController = Cast<APlayerController>(GetController()))
		{
			if (const auto InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				InputSubsystem->AddMappingContext(InputMapping.LoadSynchronous(), 0);
			}
		}
	}
}

void AVRPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateWidgetBeam();
	ShowHideControllers();
}

void AVRPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (const auto EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MenuAction, ETriggerEvent::Completed, this, &AVRPawn::InputMenu);
		EnhancedInputComponent->BindAction(ClickAction, ETriggerEvent::Triggered, this, &AVRPawn::InputClick);
		EnhancedInputComponent->BindAction(ClickAction, ETriggerEvent::Completed, this, &AVRPawn::InputRelease);
	}
}

void AVRPawn::InputMenu(const FInputActionValue& Value)
{
	const bool Pressed = Value.Get<bool>();
	// Toggle visibility of DemoMenu
	DemoMenu->SetVisibility(!DemoMenu->IsVisible(), Pressed);
}

void AVRPawn::InputClick(const FInputActionValue& Value)
{
	WidgetInteraction->PressPointerKey(EKeys::LeftMouseButton);
}

void AVRPawn::InputRelease(const FInputActionValue& Value)
{
	WidgetInteraction->ReleasePointerKey(EKeys::LeftMouseButton);
}

void AVRPawn::UpdateWidgetBeam() const
{
	if (WidgetInteraction->bEnableHitTesting && WidgetInteraction->IsOverHitTestVisibleWidget())
	{
		const FVector HitLocation = WidgetInteraction->GetLastHitResult().Location;
		Spline->SetLocationAtSplinePoint(1, HitLocation, ESplineCoordinateSpace::World, true);
	}
	else
	{
		const FVector Location = Spline->GetLocationAtSplinePoint(0, ESplineCoordinateSpace::World);
		Spline->SetLocationAtSplinePoint(1, Location, ESplineCoordinateSpace::World, true);
	}

	FVector StartLocation{};
	FVector StartTangent{};
	Spline->GetLocationAndTangentAtSplinePoint(0, StartLocation, StartTangent, ESplineCoordinateSpace::Local);

	FVector EndLocation{};
	FVector EndTangent{};
	Spline->GetLocationAndTangentAtSplinePoint(1, EndLocation, EndTangent, ESplineCoordinateSpace::Local);

	SplineMesh->SetStartAndEnd(StartLocation, StartTangent, EndLocation, EndTangent, true);
}

void AVRPawn::ShowHideControllers() const
{
	// Hide controllers if game is paused
	const bool HasInputFocus = UOculusXRFunctionLibrary::HasInputFocus();
	MotionControllerLeft->SetHiddenInGame(!HasInputFocus, true);
	MotionControllerRight->SetHiddenInGame(!HasInputFocus, true);
}
