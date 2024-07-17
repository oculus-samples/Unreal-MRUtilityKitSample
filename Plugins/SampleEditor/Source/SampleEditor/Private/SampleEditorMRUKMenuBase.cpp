/*
Copyright (c) Meta Platforms, Inc. and affiliates.
All rights reserved.

This source code is licensed under the license found in the
LICENSE file in the root directory of this source tree.
*/
#include "SampleEditorMRUKMenuBase.h"

#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"

void USampleEditorMRUKMenuBase::NativeConstruct()
{
	Super::NativeConstruct();

	// Construct the path to the menu widget with the help of the currently loaded map

	const FString MapName = GetWorld()->GetMapName();
	const FString WidgetPath = "/Game/Sample" + MapName + "/" + "WBP_Menu" + MapName + ".WBP_Menu" + MapName + "_C";
	UClass* WidgetClass = LoadObject<UClass>(nullptr, *WidgetPath);
	if (!WidgetClass)
	{
		return;
	}

	UUserWidget* WidgetInstance = CreateWidget<UUserWidget>(this, WidgetClass);
	if (!WidgetClass)
	{
		return;
	}
	const auto Root = Cast<UVerticalBox>(GetRootWidget());
	check(Root);
	UVerticalBoxSlot* VerticalSlot = Root->AddChildToVerticalBox(WidgetInstance);
	VerticalSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
}
