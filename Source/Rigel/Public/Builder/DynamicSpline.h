// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h"

#include "DynamicSpline.generated.h"

UCLASS()
class RIGEL_API ADynamicSpline : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADynamicSpline();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category= "Dynamic GC")
	virtual FVector GetPoint(const int32 PointIndex);
	
	UFUNCTION(BlueprintCallable, Category= "Dynamic GC")
	virtual void AddPoint(const FVector& Location);

	UFUNCTION(BlueprintCallable, Category= "Dynamic GC")
	virtual void SetPoint(const int32 PointIndex, const FVector& Location);

	UFUNCTION(BlueprintCallable, Category= "Dynamic GC")
	virtual void PopPoint();
	
	UFUNCTION(BlueprintCallable, Category= "Dynamic GC")
	virtual void ClearPoints();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category= "Dynamic GC")
	void BuildMesh();

	UFUNCTION(BlueprintCallable, Category= "Dynamic GC")
	virtual void UpdateMesh();

	UFUNCTION(BlueprintCallable, Category= "Dynamic GC")
	int32 GetPointsNumber();

	UFUNCTION(BlueprintCallable, Category= "Dynamic GC")
	void SetClosedLoop(bool IsClose);
	
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USplineComponent* Spline;
};
