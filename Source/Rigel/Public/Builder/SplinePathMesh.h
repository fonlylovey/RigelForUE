// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineMeshComponent.h"
#include "SplinePathMesh.generated.h"

class USplineComponent;

UCLASS()
class EARTHBUILDER_API ASplinePathMesh : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASplinePathMesh();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
    
    virtual void OnConstruction(const FTransform& Transform) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable)
    void UpdateSplineMesh();

    UFUNCTION(BlueprintCallable)
    void AddPoint(const FVector& location);

    UFUNCTION(BlueprintCallable)
    void SetGrow(float GrowValue= 0.1);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EarthBuilder")
    USplineComponent* Spline;

    //The mesh used by the spline
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EarthBuilder")
    UStaticMesh* SplineMesh;

    //The material used by the spline
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EarthBuilder")
    UMaterial* SplineMaterial;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EarthBuilder")
    float Width = 1.0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EarthBuilder", meta = (ClampMin = 0.0, ClampMax = 1.0))
    float Opacity = 0.5;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EarthBuilder", meta = (ClampMin = 0.0, ClampMax = 100.0))
    float Light = 1.0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EarthBuilder")
    FLinearColor Color = FColor::Blue;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EarthBuilder", meta = (ClampMin = 0.0, ClampMax = 1.0))
    float Grow = 0.1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EarthBuilder")
	TEnumAsByte<ESplineMeshAxis::Type> MeshAxis = ESplineMeshAxis::X;
private:
    
};
