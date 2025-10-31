// Copyright (c) Meta Platforms, Inc. and affiliates.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "RoomMesh.generated.h"

UCLASS()
class ARoomMesh : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* Material = nullptr;

	ARoomMesh();

	void BeginPlay() override;

private:
	UPROPERTY()
	TObjectPtr<UProceduralMeshComponent> ProceduralMesh = nullptr;

	UFUNCTION()
	void OnRoomCreated(AMRUKRoom* Room);

	void GenerateProceduralMeshes(AMRUKRoom* Room);
};
