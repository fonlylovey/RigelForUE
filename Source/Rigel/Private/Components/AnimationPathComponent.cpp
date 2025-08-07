// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/AnimationPathComponent.h"
#include "Components/SplineComponent.h"

// Sets default values for this component's properties
UAnimationPathComponent::UAnimationPathComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UAnimationPathComponent::PlayAnimationPath_Implementation()
{
    PausePlay = false;
}

void UAnimationPathComponent::StopAnimationPath_Implementation()
{
    PausePlay = true;
}

void UAnimationPathComponent::ResetAnimationPath_Implementation()
{
    InitAnimation();
    PausePlay = true;
}

// Called when the game starts
void UAnimationPathComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UAnimationPathComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
    if (!PausePlay)
    {
        RunningTime += DeltaTime;
        float Duration = Spline->GetSplineLength() / Speed / 100.0;
        FVector location = Spline->GetLocationAtTime(RunningTime / Duration, ESplineCoordinateSpace::World);
        FRotator rotator = Spline->GetTangentAtTime(RunningTime / Duration, ESplineCoordinateSpace::World).Rotation();
        GetOwner()->SetActorLocationAndRotation(location, rotator);
        if (RunningTime > Duration)
        {
            PausePlay = true;
            RunningTime = 0.0;
            if (LoopPlay)
            {
                PausePlay = false;
                InitAnimation();
            }
        }
    }
}

#if WITH_EDITOR

void UAnimationPathComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    FName propertyName = PropertyChangedEvent.Property->GetFName();

    if (propertyName == GET_MEMBER_NAME_CHECKED(UAnimationPathComponent, SplineActor))
    {
       if (SplineActor)
       {
           InitAnimation();
       }
    }
    Super::PostEditChangeProperty(PropertyChangedEvent);
}

void UAnimationPathComponent::InitAnimation()
{
    Spline = SplineActor->Spline;
    if (Spline!= nullptr && Spline->GetNumberOfSplinePoints() > 1)
    {
        FVector startLocation = Spline->GetLocationAtSplinePoint(0, ESplineCoordinateSpace::World);
        GetOwner()->SetActorLocation(startLocation);
    }
}

#endif


