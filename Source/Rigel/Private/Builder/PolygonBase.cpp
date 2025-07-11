// Fill out your copyright notice in the Description page of Project Settings.


#include "Builder/PolygonBase.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/DecalComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
APolygonBase::APolygonBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	Spline->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	
	CaptureCamera = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("CaptureCamera"));
	CaptureCamera->SetMobility(EComponentMobility::Movable);
	CaptureCamera->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
    CaptureCamera->CaptureSource = ESceneCaptureSource::SCS_BaseColor;
	CaptureCamera->SetRelativeRotation(FRotator(-90, 0, -90));
	CaptureCamera->SetRelativeLocation(FVector(0, 0, 2000));
		
	
	CaptureCamera->ProjectionType = ECameraProjectionMode::Orthographic;
	CaptureCamera->PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_UseShowOnlyList;
		
	DecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComponent"));
	DecalComponent->SetMobility(EComponentMobility::Movable);
	DecalComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	DecalComponent->SetRelativeRotation(FRotator(-90, 0, 0));
	
}

// Called when the game starts or when spawned
void APolygonBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void APolygonBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
    if (!IsValid(RenderTarget))
    {
        RenderTarget = UKismetRenderingLibrary::CreateRenderTarget2D(this, 1024, 1024, ETextureRenderTargetFormat::RTF_R8);
        CaptureCamera->TextureTarget = RenderTarget;
    }
	//无论mesh还是贴地都要绘制几何
	BuildMesh();

	if (DrawMode == EDrawMode::Overlay)
	{
		BuildDecal();
	}
}

// Called every frame
void APolygonBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APolygonBase::BuildDecal()
{
	float SplineMax = CalcSplineMaxValue();
	DecalComponent->SetRelativeScale3D(FVector(1000000,  SplineMax / DecalComponent->DecalSize.Y, SplineMax / DecalComponent->DecalSize.Z));
	CaptureCamera->OrthoWidth = SplineMax * 2;

	if (!IsValid(DecalMaterialIns) && IsValid(DecalMaterial))
	{
		DecalMaterialIns = UMaterialInstanceDynamic::Create(DecalMaterial, this);
		DecalComponent->SetDecalMaterial(DecalMaterialIns);
	}
	if (IsValid(DecalMaterialIns))
	{
		DecalMaterialIns->SetTextureParameterValue("Tex", RenderTarget);
	}
}

float APolygonBase::CalcSplineMaxValue()
{
	float MaxValue = 0;
	int32 pointNum = Spline->GetNumberOfSplinePoints();
    for (int i = 0; i < pointNum - 1; i++)
    {
        FVector location = Spline->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Local);
        location.X = UKismetMathLibrary::Abs(location.X);
    	location.Y = UKismetMathLibrary::Abs(location.Y);
    	MaxValue = UKismetMathLibrary::Max(MaxValue, location.X);
    	MaxValue = UKismetMathLibrary::Max(MaxValue, location.Y);
    }
	return MaxValue;
}
