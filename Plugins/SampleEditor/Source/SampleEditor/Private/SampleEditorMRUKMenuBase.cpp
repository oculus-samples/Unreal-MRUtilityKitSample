/*
Copyright (c) Meta Platforms, Inc. and affiliates.
All rights reserved.

This source code is licensed under the license found in the
LICENSE file in the root directory of this source tree.
*/

#include "SampleEditorMRUKMenuBase.h"

#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"

// Constants for widget path construction
static const TCHAR* const GameSamplePath = TEXT("/Game/Sample");
static const TCHAR* const WidgetPrefix = TEXT("WBP_Menu");
static const TCHAR* const WidgetSuffix = TEXT("_C");

/**
 * Constructs the native widget by dynamically loading and creating a map-specific menu widget.
 * The widget path is constructed based on the currently loaded map name.
 */
void USampleEditorMRUKMenuBase::NativeConstruct()
{
	Super::NativeConstruct();

	// Get the current map name to construct the widget path dynamically
	const FString MapName = GetWorld()->GetMapName();

	// Construct the full widget path: /Game/Sample{MapName}/WBP_Menu{MapName}.WBP_Menu{MapName}_C
	const FString WidgetPath = FString(GameSamplePath) + MapName + TEXT("/") + WidgetPrefix + MapName + TEXT(".") + WidgetPrefix + MapName + WidgetSuffix;

	// Attempt to load the widget class from the constructed path
	UClass* WidgetClass = LoadObject<UClass>(nullptr, *WidgetPath);
	if (!WidgetClass)
	{
		return;
	}

	// Create an instance of the loaded widget class
	UUserWidget* WidgetInstance = CreateWidget<UUserWidget>(this, WidgetClass);
	if (!WidgetInstance)
	{
		return;
	}

	// Add the widget instance to the root vertical box and configure it to fill available space
	UVerticalBox* RootVerticalBox = Cast<UVerticalBox>(GetRootWidget());
	check(RootVerticalBox);
	UVerticalBoxSlot* VerticalSlot = RootVerticalBox->AddChildToVerticalBox(WidgetInstance);
	VerticalSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
}
