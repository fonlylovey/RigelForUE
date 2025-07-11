// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PolygonBase.h"
#include "PolygonStrip.generated.h"

class USplineMeshComponent;
/**
 * 
 */
UCLASS()
class EARTHBUILDER_API APolygonStrip : public APolygonBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APolygonStrip();
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
    
	virtual void OnConstruction(const FTransform& Transform) override;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BuildMesh() override;

private:
	void DestoryMesh();
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EarthBuilder")
	float Width = 1.0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EarthBuilder")
	FLinearColor Color = FColor::Blue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EarthBuilder")
	UStaticMesh* SplineMesh;
	
};
