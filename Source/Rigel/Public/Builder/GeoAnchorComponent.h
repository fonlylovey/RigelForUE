// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GeoAnchorComponent.generated.h"

class AGeoReferencingSystem;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EARTHBUILDER_API UGeoAnchorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGeoAnchorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

    virtual void OnRegister() override;

    virtual void OnUnregister() override;

    virtual void InitializeComponent() override;

    virtual void OnComponentCreated() override;

#if WITH_EDITOR
    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UPROPERTY(BlueprintReadOnly)
    AGeoReferencingSystem* GeoReference;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EarthBuilder")
	double Longitude;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EarthBuilder")
	double Latitude;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EarthBuilder")
	double Altitude;    

private:
    void applyGeoToEngine();

    //
    void onActorTransformChanged(
        USceneComponent* InRootComponent,
        EUpdateTransformFlags UpdateTransformFlags,
        ETeleportType Teleport);

private:
    bool isUpdatedActorTransform = false;
    FDelegateHandle updatedTransformDelegate;
};
