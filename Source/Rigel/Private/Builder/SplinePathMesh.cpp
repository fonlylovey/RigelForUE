// Fill out your copyright notice in the Description page of Project Settings.


#include "SplinePathMesh.h"
#include "Components/SplineComponent.h"
#include "Materials/MaterialInstanceDynamic.h"

// Sets default values
ASplinePathMesh::ASplinePathMesh()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
    Spline->SetMobility(EComponentMobility::Movable);
    Spline->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ASplinePathMesh::BeginPlay()
{
	Super::BeginPlay();
    UpdateSplineMesh();
}

void ASplinePathMesh::OnConstruction(const FTransform& Transform)
{
    UpdateSplineMesh();
}

// Called every frame
void ASplinePathMesh::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASplinePathMesh::UpdateSplineMesh()
{
    if (IsValid(Spline))
    {
        int32 pointNum = Spline->GetNumberOfSplinePoints();
        ClearInstanceComponents(true);
        float totalLength = Spline->GetSplineLength();
        for (int i = 0; i < pointNum - 1; i++)
        {
            FVector pointA, pointB, tangentA, tangentB;
            Spline->GetLocationAndTangentAtSplinePoint(i, pointA, tangentA, ESplineCoordinateSpace::Local);
            Spline->GetLocationAndTangentAtSplinePoint(i + 1, pointB, tangentB, ESplineCoordinateSpace::Local);
            
            float distanceStrat = Spline->GetDistanceAlongSplineAtSplinePoint(i);
            float distanceEnd = Spline->GetDistanceAlongSplineAtSplinePoint(i + 1);
            UMaterialInstanceDynamic* segmentMaterial = nullptr;
            if (SplineMaterial != nullptr)
            {
                segmentMaterial = UMaterialInstanceDynamic::Create(SplineMaterial, this);
                float UVSatrt = distanceStrat / totalLength;
                float UVEnd = distanceEnd / totalLength;
                segmentMaterial->SetScalarParameterValue("Start", UVSatrt);
                segmentMaterial->SetScalarParameterValue("End", UVEnd);
                segmentMaterial->SetScalarParameterValue("BaseOpacity", Opacity);
                segmentMaterial->SetScalarParameterValue("Light", Light);
                segmentMaterial->SetVectorParameterValue("Color", Color);
                segmentMaterial->SetScalarParameterValue("Grow", Grow);
            }

            USplineMeshComponent* segmentMesh = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());
            segmentMesh->SetMobility(EComponentMobility::Movable);
            segmentMesh->SetupAttachment(Spline);
            
            segmentMesh->SetStaticMesh(SplineMesh);
            
            segmentMesh->SetMaterial(0, segmentMaterial);
            segmentMesh->SetStartAndEnd(pointA, tangentA, pointB, tangentB);
            segmentMesh->SetStartScale(FVector2d(Width, Width), true);
            segmentMesh->SetEndScale(FVector2d(Width, Width), true);
            segmentMesh->SetForwardAxis(MeshAxis);
            segmentMesh->AttachToComponent(Spline, FAttachmentTransformRules::KeepRelativeTransform);
            AddInstanceComponent(segmentMesh);
            segmentMesh->RegisterComponent();
        }
    }
}

void ASplinePathMesh::AddPoint(const FVector& location)
{
    int32 inKey = Spline->GetNumberOfSplinePoints();
    FSplinePoint point = FSplinePoint((float)inKey, location, FVector::ZeroVector, FVector::ZeroVector, FRotator::ZeroRotator, FVector(1.0), ESplinePointType::Linear);
    Spline->AddPoint(point);
}

void ASplinePathMesh::SetGrow(float GrowValue /*= 0.1*/)
{
    Grow = GrowValue;
    if (GetWorld()->HasBegunPlay())
    {
        TArray<UActorComponent*> MeshArray = GetInstanceComponents();
        for (UActorComponent* meshComponrnt : MeshArray)
        {
            USplineMeshComponent* segmentMesh = Cast<USplineMeshComponent>(meshComponrnt);
            if (segmentMesh != nullptr)
            {
                UMaterialInstanceDynamic* segmentMaterial = Cast<UMaterialInstanceDynamic>(segmentMesh->GetMaterial(0));
                if (segmentMaterial)
                {
                    segmentMaterial->SetScalarParameterValue("Grow", Grow);
                }
            }
        }
    }
}

