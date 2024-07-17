/*
Copyright (c) Meta Platforms, Inc. and affiliates.
All rights reserved.

This source code is licensed under the license found in the
LICENSE file in the root directory of this source tree.
*/

#include "SampleBPFLibrary.h"
#include "Kismet/GameplayStatics.h"
#if WITH_EDITOR
#include "Subsystems/UnrealEditorSubsystem.h"
#endif

UWorld* USampleBPFLibrary::GetActiveGameWorld(const UObject* WorldContext)
{
#if WITH_EDITOR
	if (GEditor)
	{
		const auto EditorSubsystem = GEditor->GetEditorSubsystem<UUnrealEditorSubsystem>();
		return EditorSubsystem->GetGameWorld();
	}
#endif
	if (WorldContext)
	{
		return WorldContext->GetWorld();
	}
	return nullptr;
}

UMRUKSubsystem* USampleBPFLibrary::GetMRUKSubsystemActiveGameWorld(const UObject* WorldContext)
{
	if (const UWorld* World = GetActiveGameWorld(WorldContext))
	{
		if (const UGameInstance* GameInstance = World->GetGameInstance())
		{
			return GameInstance->GetSubsystem<UMRUKSubsystem>();
		}
	}
	return nullptr;
}

AActor* USampleBPFLibrary::GetActorOfClassActiveGameWorld(TSubclassOf<AActor> ActorClass, const UObject* WorldContext)
{
	return UGameplayStatics::GetActorOfClass(GetActiveGameWorld(WorldContext), ActorClass);
}

void USampleBPFLibrary::ExecuteConsoleCommandActiveGameWorld(const FString& Command, APlayerController* Player, const UObject* WorldContext)
{
	return UKismetSystemLibrary::ExecuteConsoleCommand(GetActiveGameWorld(WorldContext), Command, Player);
}
