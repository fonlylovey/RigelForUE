// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/QuadtreeSceneComponent.h"
#include "Camera/PlayerCameraManager.h"

// Sets default values for this component's properties
UQuadtreeSceneComponent::UQuadtreeSceneComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UQuadtreeSceneComponent::BeginPlay()
{
	Super::BeginPlay();

    LevelInterval = RootHeight / MaxLevel;
	// ...
    // GetPlayerPawn()
}


// Called every frame
void UQuadtreeSceneComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UQuadtreeSceneComponent::OnPawnLocationChange(const FVector& GeoLocation)
{
    int32 GeoHeight = GeoLocation.Z;
    int level = FMath::Min(MaxLevel, GeoHeight / LevelInterval);

}


