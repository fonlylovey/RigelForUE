// Fill out your copyright notice in the Description page of Project Settings.


#include "Builder/PolygonStrip.h"

#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Components/SplineMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"

APolygonStrip::APolygonStrip()
{
	Spline->SetClosedLoop(false);
}

void APolygonStrip::BeginPlay()
{
	Super::BeginPlay();
}

void APolygonStrip::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);
}

void APolygonStrip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APolygonStrip::BuildMesh()
{
	int32 pointNum = Spline->GetNumberOfSplinePoints();
	DestoryMesh();
	
    float totalLength = Spline->GetSplineLength();
    for (int i = 0; i < pointNum - 1; i++)
    {
        FVector pointA, pointB, tangentA, tangentB;
        Spline->GetLocationAndTangentAtSplinePoint(i, pointA, tangentA, ESplineCoordinateSpace::Local);
        Spline->GetLocationAndTangentAtSplinePoint(i + 1, pointB, tangentB, ESplineCoordinateSpace::Local);
        
        //float distanceStrat = Spline->GetDistanceAlongSplineAtSplinePoint(i);
        //float distanceEnd = Spline->GetDistanceAlongSplineAtSplinePoint(i + 1);
    	if (!IsValid(MeshMaterialIns) && IsValid(MeshMaterial))
        {
            MeshMaterialIns = UMaterialInstanceDynamic::Create(MeshMaterial, this);
        }

    	//float UVSatrt = distanceStrat / totalLength;
    	//float UVEnd = distanceEnd / totalLength;
    	//segmentMaterial->SetScalarParameterValue("Start", UVSatrt);
    	//segmentMaterial->SetScalarParameterValue("End", UVEnd);
    	
        USplineMeshComponent* segmentMesh = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());
        segmentMesh->SetMobility(EComponentMobility::Movable);
        segmentMesh->SetupAttachment(Spline);
        
        segmentMesh->SetStaticMesh(SplineMesh);
        segmentMesh->SetStartAndEnd(pointA, tangentA, pointB, tangentB);
        segmentMesh->SetStartScale(FVector2d(Width, Width), true);
        segmentMesh->SetEndScale(FVector2d(Width, Width), true);
        segmentMesh->SetForwardAxis(ESplineMeshAxis::X);
        segmentMesh->AttachToComponent(Spline, FAttachmentTransformRules::KeepRelativeTransform);
        AddInstanceComponent(segmentMesh);
    	if (DrawMode == EDrawMode::Overlay)
    	{
    		segmentMesh->SetMaterial(0, nullptr);
    		segmentMesh->SetVisibleInSceneCaptureOnly(true);
    		CaptureCamera->ShowOnlyComponent(segmentMesh);
    	}
	    else
	    {
	    	//UMaterialInstanceDynamic::Create(MeshMaterial, this);
	    	segmentMesh->SetMaterial(0, MeshMaterialIns);
	    	MeshMaterialIns->SetScalarParameterValue("Opacity", 1.0);
	    	MeshMaterialIns->SetVectorParameterValue("Color", Color);
	    }
    }
	if (DrawMode == EDrawMode::Overlay)
	{
		BuildDecal();
	}
	
}

void APolygonStrip::DestoryMesh()
{
	TArray<USceneComponent*> MeshComponents;
	Spline->GetChildrenComponents(true, MeshComponents);
	for (auto mesh : MeshComponents)
	{
        RemoveInstanceComponent(mesh);
		mesh->DestroyComponent();
	}
	MeshComponents.Empty();
}

