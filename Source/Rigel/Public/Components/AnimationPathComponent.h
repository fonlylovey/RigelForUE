// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Builder/SplinePathMesh.h"
#include "AnimationPathComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RIGEL_API UAnimationPathComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAnimationPathComponent();

public:
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void PlayAnimationPath();

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void StopAnimationPath();

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void ResetAnimationPath();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
     
#if WITH_EDITOR
    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

private:
    void InitAnimation();

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rigel")
    ASplinePathMesh* SplineActor;

    UPROPERTY()
    USplineComponent* Spline;

    // �Ƿ�ѭ���˶�
    UPROPERTY(EditAnywhere, Category = "Rigel")
    bool LoopPlay = false;

    // ��ͣ���ţ����ص���ʼλ��
    UPROPERTY(EditAnywhere, Category = "Rigel")
    bool PausePlay = true;

    // ��ͣ���ţ����ص���ʼλ��
    UPROPERTY(EditAnywhere, Category = "Rigel")
    float Speed = 50.0;

    // ��ͣ���ţ����ص���ʼλ��
    UPROPERTY()
    float RunningTime = 0.0;
};
