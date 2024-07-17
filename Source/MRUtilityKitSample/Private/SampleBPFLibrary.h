/*
Copyright (c) Meta Platforms, Inc. and affiliates.
All rights reserved.

This source code is licensed under the license found in the
LICENSE file in the root directory of this source tree.
*/

#pragma once

#include "MRUtilityKitSubsystem.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SampleBPFLibrary.generated.h"

UCLASS()
class USampleBPFLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Sample", meta = (WorldContext = "WorldContext"))
	static UWorld* GetActiveGameWorld(const UObject* WorldContext);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Sample", meta = (WorldContext = "WorldContext"))
	static UMRUKSubsystem* GetMRUKSubsystemActiveGameWorld(const UObject* WorldContext);

	UFUNCTION(BlueprintCallable, Category = "Sample", meta = (WorldContext = "WorldContext"))
	static AActor* GetActorOfClassActiveGameWorld(TSubclassOf<AActor> ActorClass, const UObject* WorldContext);

	UFUNCTION(BlueprintCallable, Category = "Sample", meta = (WorldContext = "WorldContext"))
	static void ExecuteConsoleCommandActiveGameWorld(const FString& Command, APlayerController* Player, const UObject* WorldContext);
};
