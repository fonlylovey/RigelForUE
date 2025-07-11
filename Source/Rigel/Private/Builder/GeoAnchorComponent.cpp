// Fill out your copyright notice in the Description page of Project Settings.


#include "Builder/GeoAnchorComponent.h"
#include "GeoReferencingSystem.h"

// Sets default values for this component's properties
UGeoAnchorComponent::UGeoAnchorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
    bWantsInitializeComponent = true;
}


// Called when the game starts
void UGeoAnchorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
    if (!IsValid(GeoReference))
    {
        GeoReference = AGeoReferencingSystem::GetGeoReferencingSystem(GWorld);
    }
}


void UGeoAnchorComponent::OnRegister()
{
    Super::OnRegister();
    if (!IsValid(GeoReference))
    {
        GeoReference = AGeoReferencingSystem::GetGeoReferencingSystem(GWorld);
    }

    if (Longitude != 0.f && Latitude != 0.f && Altitude)
    {
        applyGeoToEngine();
    }
    const AActor* pOwner = this->GetOwner();
    if (IsValid(pOwner)) 
    {
        USceneComponent* pOwnerRoot = pOwner->GetRootComponent();
        if (pOwnerRoot) 
        {
            if (updatedTransformDelegate.IsValid()) 
            {
                pOwnerRoot->TransformUpdated.Remove(updatedTransformDelegate);
            }
            pOwnerRoot->TransformUpdated.AddUObject(
                this,
                &UGeoAnchorComponent::onActorTransformChanged);
        }
    }
}

void UGeoAnchorComponent::OnUnregister()
{
    Super::OnUnregister();
    const AActor* pOwner = this->GetOwner();
    if (IsValid(pOwner))
    {
        USceneComponent* pOwnerRoot = pOwner->GetRootComponent();
        if (pOwnerRoot)
        {
            if (updatedTransformDelegate.IsValid()) 
            {
                pOwnerRoot->TransformUpdated.Remove(updatedTransformDelegate);
            }
        }
    }
}

void UGeoAnchorComponent::InitializeComponent()
{
    Super::InitializeComponent();
}

void UGeoAnchorComponent::OnComponentCreated()
{
    Super::OnComponentCreated();
    const AActor* pOwner = this->GetOwner();
    if (IsValid(pOwner))
    {
        USceneComponent* pOwnerRoot = pOwner->GetRootComponent();
        if (pOwnerRoot)
        {
            onActorTransformChanged(pOwnerRoot, EUpdateTransformFlags::SkipPhysicsUpdate, ETeleportType::None);
        }
    }
}

#if WITH_EDITOR
void UGeoAnchorComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);
    if (!PropertyChangedEvent.Property) 
    {
        return;
    }

    FName propertyName = PropertyChangedEvent.Property->GetFName();

    if (propertyName ==
        GET_MEMBER_NAME_CHECKED(UGeoAnchorComponent, Longitude) ||
        propertyName ==
        GET_MEMBER_NAME_CHECKED(UGeoAnchorComponent, Latitude) ||
        propertyName ==
        GET_MEMBER_NAME_CHECKED(UGeoAnchorComponent, Altitude)) 
    {
        applyGeoToEngine();
    }
}
#endif
// Called every frame
void UGeoAnchorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UGeoAnchorComponent::applyGeoToEngine()
{
    if (IsValid(GeoReference) && !isUpdatedActorTransform)
    {
        FVector ueWorldLocation;
        GeoReference->GeographicToEngine(FGeographicCoordinates(Longitude, Latitude, Altitude), ueWorldLocation);
        GetOwner()->SetActorLocation(ueWorldLocation);
    }
}

void UGeoAnchorComponent::onActorTransformChanged(USceneComponent* InRootComponent, EUpdateTransformFlags UpdateTransformFlags, ETeleportType Teleport)
{
    isUpdatedActorTransform = true;
    if (IsValid(GeoReference))
    {
        FGeographicCoordinates geoPosition;
        GeoReference->EngineToGeographic(InRootComponent->GetComponentLocation(), geoPosition);
        Longitude = geoPosition.Longitude;
        Latitude = geoPosition.Latitude;
        Altitude = geoPosition.Altitude;
    }
    isUpdatedActorTransform = false;
}

