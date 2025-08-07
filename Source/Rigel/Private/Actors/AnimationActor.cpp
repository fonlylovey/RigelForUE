// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/AnimationActor.h"

// Sets default values
AAnimationActor::AAnimationActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAnimationActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAnimationActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    if (IsPlaying)
    {
        RunningTime += DeltaTime;
        AnimationEvent();
    }
}

void AAnimationActor::PlayAnimation_Implementation(bool isPlaying, float PlayDuration)
{
    IsPlaying = isPlaying;
    Duration = PlayDuration;
}

void AAnimationActor::ResetAnimation_Implementation()
{
    IsPlaying = false;
    RunningTime = 0.0;
}

void AAnimationActor::AnimationEvent_Implementation()
{

}

