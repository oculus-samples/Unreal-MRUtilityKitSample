/*
Copyright (c) Meta Platforms, Inc. and affiliates.
All rights reserved.

This source code is licensed under the license found in the
LICENSE file in the root directory of this source tree.
*/

#include "RoomMesh.h"
#include "MRUtilityKitSubsystem.h"

ARoomMesh::ARoomMesh()
{
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;
	RootComponent->SetMobility(EComponentMobility::Movable);
}

void ARoomMesh::BeginPlay()
{
	Super::BeginPlay();

	UMRUKSubsystem* Subsystem = GetGameInstance()->GetSubsystem<UMRUKSubsystem>();
	if (!Subsystem)
	{
		return;
	}
	if (Subsystem->SceneLoadStatus == EMRUKInitStatus::Complete)
	{
		GenerateProceduralMeshes(Subsystem->GetCurrentRoom());
	}
	else
	{
		Subsystem->OnRoomCreated.AddUniqueDynamic(this, &ARoomMesh::OnRoomCreated);
	}
}

void ARoomMesh::OnRoomCreated(AMRUKRoom* Room)
{
	if (!ProceduralMesh)
	{
		GenerateProceduralMeshes(Room);
	}
}

void ARoomMesh::GenerateProceduralMeshes(const AMRUKRoom* Room)
{
	if (!Room)
	{
		return;
	}
	const UMRUKRoomMesh* RoomMesh = Room->RoomMesh;
	if (!RoomMesh)
	{
		return;
	}

	// Copy transform from room
	SetActorTransform(Room->GetActorTransform());

	ProceduralMesh = NewObject<UProceduralMeshComponent>(this, TEXT("ProceduralMesh"));
	ProceduralMesh->SetupAttachment(GetRootComponent());
	ProceduralMesh->RegisterComponent();
	AddInstanceComponent(ProceduralMesh);

	int32 SectionIndex = 0;

	for (const FMRUKRoomFace& RoomFace : RoomMesh->Faces)
	{
		TArray<FVector> Vertices;
		TArray<int32> Indices;
		TArray<FVector> Normals;
		TArray<FVector2D> UVs;
		TArray<FLinearColor> Colors;
		TArray<FProcMeshTangent> Tangents;

		Vertices.Reserve(RoomFace.Indices.Num());
		Indices.Reserve(RoomFace.Indices.Num());
		Colors.Reserve(RoomFace.Indices.Num());

		// Assign colors based on semantic label
		FLinearColor Color = FLinearColor::Black;
		if (RoomFace.SemanticClassification == FMRUKLabels::Floor)
		{
			Color = FLinearColor(0.2f, 0.6f, 0.2f, 1.0f); // Green for floor
		}
		else if (RoomFace.SemanticClassification == FMRUKLabels::Ceiling)
		{
			Color = FLinearColor(0.8f, 0.8f, 0.8f, 1.0f); // White for ceiling
		}
		else if (RoomFace.SemanticClassification == FMRUKLabels::WallFace)
		{
			Color = FLinearColor(0.6f, 0.6f, 0.8f, 1.0f); // Blue for walls
		}
		else if (RoomFace.SemanticClassification == FMRUKLabels::InvisibleWallFace)
		{
			Color = FLinearColor(0.8f, 0.3f, 0.8f, 1.0f); // Purple for invisible walls
		}
		else if (RoomFace.SemanticClassification == FMRUKLabels::InnerWallFace)
		{
			Color = FLinearColor(0.4f, 0.4f, 0.6f, 1.0f); // Darker blue for inner walls
		}
		else if (RoomFace.SemanticClassification == FMRUKLabels::WindowFrame)
		{
			Color = FLinearColor(0.7f, 0.9f, 1.0f, 1.0f); // Light blue for windows
		}
		else if (RoomFace.SemanticClassification == FMRUKLabels::DoorFrame)
		{
			Color = FLinearColor(0.6f, 0.4f, 0.2f, 1.0f); // Brown for doors
		}

		// Fill in the vertices and indices
		for (int32 Index : RoomFace.Indices)
		{
			if (RoomMesh->Vertices.IsValidIndex(Index))
			{
				Vertices.Add(RoomMesh->Vertices[Index]);
				Indices.Add(Indices.Num()); // Add the current index in the Vertices array
				Colors.Add(Color);
			}
		}

		// Create the mesh section
		ProceduralMesh->CreateMeshSection_LinearColor(
			SectionIndex,
			Vertices,
			Indices,
			Normals,
			UVs,
			Colors,
			Tangents,
			false);

		if (Material)
		{
			ProceduralMesh->SetMaterial(SectionIndex, Material);
		}

		++SectionIndex;
	}

	ProceduralMesh->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
}
