// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DynamicSpline.h"
#include "PCGComponent.h"
#include "DynamicPCG.generated.h"

UCLASS()
class RIGEL_API ADynamicPCG : public ADynamicSpline
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADynamicPCG();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void UpdateMesh() override;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnSpace = 1000.0;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UPCGComponent* PCGComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector PCGLocation;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector PCGDirection;
};
