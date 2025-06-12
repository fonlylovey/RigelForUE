#include "Components/ViewpointComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"

// Sets default values for this component's properties
UViewpointComponent::UViewpointComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}

// Called when the game starts
void UViewpointComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}

// Called every frame
void UViewpointComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UViewpointComponent::AddViewpoint()
{
    FViewpoint viewpoint;
    viewpoint.Location = GetOwner()->GetActorLocation();
    viewpoint.Rotation = GetOwner()->GetActorRotation();
    viewpoint.ID = MakeUniqueObjectName(this, UViewpointComponent::StaticClass(), TEXT("Viewpoint_")).ToString();
    viewpoint.Name = viewpoint.ID;
    ViewpointList.Add(viewpoint.ID, viewpoint);
}

void UViewpointComponent::RoamingToViewpoint(float time, const FString& ViewpointID)
{

}
