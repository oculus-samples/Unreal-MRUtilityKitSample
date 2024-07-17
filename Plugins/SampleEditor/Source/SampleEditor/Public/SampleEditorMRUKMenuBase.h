/*
Copyright (c) Meta Platforms, Inc. and affiliates.
All rights reserved.

This source code is licensed under the license found in the
LICENSE file in the root directory of this source tree.
*/
#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "SampleEditorMRUKMenuBase.generated.h"

UCLASS(BlueprintType, ClassGroup = MRUtilityKit, meta = (DisplayName = "Editor MRUK Menu Base"))
class SAMPLEEDITOR_API USampleEditorMRUKMenuBase : public UEditorUtilityWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
};
