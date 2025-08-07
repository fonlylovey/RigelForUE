// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AnimationActor.generated.h"

UCLASS(BlueprintType, Blueprintable)
class RIGEL_API AAnimationActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAnimationActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void PlayAnimation(bool isPlaying, float PlayDuration);

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void ResetAnimation();

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void AnimationEvent();

    UPROPERTY(BlueprintReadWrite)
    bool IsPlaying = false;

    UPROPERTY(BlueprintReadWrite)
    float RunningTime = 0.0;

    UPROPERTY(BlueprintReadWrite)
    float Duration = 0.0;
};
