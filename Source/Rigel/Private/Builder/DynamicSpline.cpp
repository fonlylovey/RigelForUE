// Fill out your copyright notice in the Description page of Project Settings.


#include "Builder/DynamicSpline.h"


// Sets default values
ADynamicSpline::ADynamicSpline()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	Spline->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	Spline->SetClosedLoop(true);
	//Spline->ClearSplinePoints();
}

// Called when the game starts or when spawned
void ADynamicSpline::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ADynamicSpline::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FVector ADynamicSpline::GetPoint(const int32 PointIndex)
{
	FVector Location = Spline->GetLocationAtSplineInputKey(PointIndex, ESplineCoordinateSpace::World);
	return Location;
}

void ADynamicSpline::AddPoint(const FVector& Location)
{
	Spline->AddSplineWorldPoint(Location);
	Spline->UpdateSpline();
	UpdateMesh();
}

void ADynamicSpline::SetPoint(const int32 PointIndex, const FVector& Location)
{
	Spline->SetWorldLocationAtSplinePoint(PointIndex, Location);
	Spline->UpdateSpline();
	UpdateMesh();
}

void ADynamicSpline::PopPoint()
{
	Spline->RemoveSplinePoint(Spline->GetNumberOfSplinePoints() - 1);
	Spline->UpdateSpline();
	UpdateMesh();
}

void ADynamicSpline::ClearPoints()
{
	Spline->ClearSplinePoints();
	Spline->UpdateSpline();
	UpdateMesh();
}

void ADynamicSpline::UpdateMesh()
{
	BuildMesh();
}

int32 ADynamicSpline::GetPointsNumber()
{
	return Spline->GetNumberOfSplinePoints();
}

void ADynamicSpline::SetClosedLoop(bool IsClose)
{
	Spline->SetClosedLoop(IsClose);
}

void ADynamicSpline::BuildMesh_Implementation()
{
	
}

