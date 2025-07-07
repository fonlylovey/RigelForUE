// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "POIBase.generated.h"

class UWidgetComponent;

UCLASS()
class RIGEL_API APOIBase : public AActor
{
    GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APOIBase();

protected:
	// Called when, the game starts or when spawned
	virtual void BeginPlay() override;
    
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

    virtual void BeginDestroy() override;

    // Called every frame
    virtual void Tick(float DeltaTime) override;

#if WITH_EDITOR
    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

    virtual void OnConstruction(const FTransform& Transform) override;

    virtual bool ShouldTickIfViewportsOnly() const override;

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EarthBuilder")
    FString ID = TEXT("");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EarthBuilder")
    float MaxScale = 10.0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EarthBuilder")
    float ScaleFactor = 100.0;

    //是否在最大距离时隐藏POI
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EarthBuilder")
    bool AtMaxHide;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EarthBuilder")
    bool IsEditorTick = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EarthBuilder")
    FString POIText = TEXT("");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EarthBuilder")
    UWidgetComponent* WidgetComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EarthBuilder")
    FVector InitPostion = FVector::ZeroVector;

private:
    void createWidget();

    //
    void updateGeo(const FTransform& Transform);

private:
    APlayerController* pPlayerController = nullptr;

};
