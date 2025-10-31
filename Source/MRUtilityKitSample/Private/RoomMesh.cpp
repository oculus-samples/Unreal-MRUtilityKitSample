// Copyright (c) Meta Platforms, Inc. and affiliates.

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

	const auto Subsystem = GetGameInstance()->GetSubsystem<UMRUKSubsystem>();
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

void ARoomMesh::GenerateProceduralMeshes(AMRUKRoom* Room)
{
	UMRUKRoomMesh* RoomMesh = Room->RoomMesh;
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

	int SectionIndex = 0;

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
		FLinearColor color;
		if (RoomFace.SemanticClassification == FMRUKLabels::Floor)
		{
			color = FLinearColor(0.2f, 0.6f, 0.2f, 1.0f); // Green for floor
		}
		else if (RoomFace.SemanticClassification == FMRUKLabels::Ceiling)
		{
			color = FLinearColor(0.8f, 0.8f, 0.8f, 1.0f); // White for ceiling
		}
		else if (RoomFace.SemanticClassification == FMRUKLabels::WallFace)
		{
			color = FLinearColor(0.6f, 0.6f, 0.8f, 1.0f); // Blue for walls
		}
		else if (RoomFace.SemanticClassification == FMRUKLabels::InvisibleWallFace)
		{
			color = FLinearColor(0.8f, 0.3f, 0.8f, 1.0f); // Purple for invisible walls
		}
		else if (RoomFace.SemanticClassification == FMRUKLabels::InnerWallFace)
		{
			color = FLinearColor(0.4f, 0.4f, 0.6f, 1.0f); // Darker blue for inner walls
		}
		else if (RoomFace.SemanticClassification == FMRUKLabels::WindowFrame)
		{
			color = FLinearColor(0.7f, 0.9f, 1.0f, 1.0f); // Light blue for windows
		}
		else if (RoomFace.SemanticClassification == FMRUKLabels::DoorFrame)
		{
			color = FLinearColor(0.6f, 0.4f, 0.2f, 1.0f); // Brown for doors
		}

		// Fill in the vertices and indices
		for (int32 Index : RoomFace.Indices)
		{
			if (RoomMesh->Vertices.IsValidIndex(Index))
			{
				Vertices.Push(RoomMesh->Vertices[Index]);
				Indices.Push(Indices.Num()); // Add the current index in the Vertices array
				Colors.Push(color);
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
