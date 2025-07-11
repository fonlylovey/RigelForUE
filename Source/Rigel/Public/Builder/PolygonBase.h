// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "PolygonBase.generated.h"

UENUM(BlueprintType)
enum class EDrawMode : uint8
{
	Overlay UMETA(DisplayName = "贴地"),

	Mesh UMETA(DisplayName = "几何")
};

UCLASS()
class EARTHBUILDER_API APolygonBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APolygonBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void BuildMesh() {};

	virtual void BuildDecal();

	float CalcSplineMaxValue();
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USplineComponent* Spline;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USceneCaptureComponent2D* CaptureCamera;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UDecalComponent* DecalComponent;
	
	UPROPERTY()
	UTextureRenderTarget2D* RenderTarget;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UMaterial* DecalMaterial;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UMaterial* MeshMaterial;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EDrawMode DrawMode = EDrawMode::Overlay;

    UPROPERTY()
	UMaterialInstanceDynamic* DecalMaterialIns;

    UPROPERTY()
	UMaterialInstanceDynamic* MeshMaterialIns;
	
};
