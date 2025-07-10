// Fill out your copyright notice in the Description page of Project Settings.


#include "AbcimGeoFunctionLibrary.h"
#include "GeoReferencingSystem.h"
#include "MeshDescription.h"
#include "StaticMeshAttributes.h"

void UAbcimGeoFunctionLibrary::ProjectMeshesToEngine(AGeoReferencingSystem* GeoReferencingSystem, UStaticMeshComponent* StaticMeshComponent, const FVector Offset)
{
	UE_LOG(LogTemp, Warning, TEXT("UAbcimGeoFunctionLibrary::ProjectMeshesToEngine"));

	FTransform ComponentToWorld = StaticMeshComponent->GetComponentTransform();
	// UE_LOG(LogTemp, Warning, TEXT("ComponentToWorld0:(%f, %f, %f)"), ComponentToWorld.GetTranslation().X, ComponentToWorld.GetTranslation().Y, ComponentToWorld.GetTranslation().Z);

	TObjectPtr<UStaticMesh> StaticMesh = StaticMeshComponent->GetStaticMesh();
	if (!IsValid(StaticMesh))
	{
		UE_LOG(LogTemp, Error, TEXT("StaticMesh is null."));
		return;
	}

	FMeshDescription* MeshDescription = StaticMesh->GetMeshDescription(0);
	if (!MeshDescription)
	{
		UE_LOG(LogTemp, Error, TEXT("MeshDescription is null."));
		return;
	}
	FStaticMeshAttributes Attributes(*MeshDescription);

	const TVertexAttributesRef<FVector3f> VertexPositions = Attributes.GetVertexPositions();
	for (const FVertexID VertexID : MeshDescription->Vertices().GetElementIDs())
	{
		const double X = VertexPositions[VertexID].X;
		const double Y = VertexPositions[VertexID].Y;
		const double Z = VertexPositions[VertexID].Z;

		const FVector CurrentWorldPosition = ComponentToWorld.TransformPosition(FVector(X, Y, Z));
		// UE_LOG(LogTemp, Warning, TEXT("CurrentWorldPosition:(%f, %f, %f)"), CurrentWorldPosition.X, CurrentWorldPosition.Y, CurrentWorldPosition.Z);
		// 在 UE 的 Projected 坐标系中，North 为 Y 轴负方向，Y 坐标为负时，位于更北的位置
		FVector ProjectedCoords = FVector(Offset.X + CurrentWorldPosition.X * 0.01,
		                                  Offset.Y - CurrentWorldPosition.Y * 0.01,
		                                  Offset.Z + CurrentWorldPosition.Z * 0.01);
		// UE_LOG(LogTemp, Warning, TEXT("ProjectedCoords0:(%f, %f, %f)"), ProjectedCoords.X, ProjectedCoords.Y, ProjectedCoords.Z);
		FVector EngineCoords = FVector::Zero();
		GeoReferencingSystem->ProjectedToEngine(ProjectedCoords, EngineCoords);
		// UE_LOG(LogTemp, Warning, TEXT("EngineCoords0:(%f, %f, %f)"), EngineCoords.X, EngineCoords.Y, EngineCoords.Z);
		VertexPositions[VertexID].X = EngineCoords.X;
		VertexPositions[VertexID].Y = EngineCoords.Y;
		VertexPositions[VertexID].Z = EngineCoords.Z;
	}
	
	StaticMesh->CommitMeshDescription(0);
	
	StaticMesh->Build();
	StaticMesh->MarkPackageDirty();
}
