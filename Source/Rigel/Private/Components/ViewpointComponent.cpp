#include "Components/ViewpointComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UViewpointComponent::UViewpointComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
    SetComponentTickEnabled(false);
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
    if (Duration > 0)
    {
        APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
        
        Runtime += DeltaTime;
        float rotio = Runtime / Duration;
        //由于rotio基本不可能刚好到达模板位置，所以结束时，设置为TargetTransform
        if (rotio >= 1.0)
        {
            SetComponentTickEnabled(false);
            if (PlayerController != nullptr)
            {
                GetOwner()->SetActorLocation(TargetTransform.GetLocation());
                PlayerController->SetControlRotation(TargetTransform.Rotator());
            }
            Duration = 0.0;
            Runtime = 0.0;
            return;
        }

        FTransform deltaTransform = UKismetMathLibrary::TLerp(StartTransform, TargetTransform, rotio, ELerpInterpolationMode::QuatInterp);
        if (PlayerController != nullptr)
        {
            GetOwner()->SetActorLocation(deltaTransform.GetLocation());
            PlayerController->SetControlRotation(deltaTransform.Rotator());
            GetOwner()->SetActorTransform(deltaTransform);
        }
    }
}

void UViewpointComponent::RoamingToViewpoint(float time, const FViewpoint& Viewpoint)
{
    AActor* owner = GetOwner();
    StartTransform = owner->GetTransform();
    Runtime = 0.0;
    Duration = time;
    TargetTransform.SetLocation(Viewpoint.Location);
    TargetTransform.SetRotation(FQuat::MakeFromRotator(Viewpoint.Rotation));
    SetComponentTickEnabled(true);
}
