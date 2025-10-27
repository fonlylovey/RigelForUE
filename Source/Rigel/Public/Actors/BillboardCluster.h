// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/RigelBillboardComponent.h"
#include "BillboardCluster.generated.h"

UCLASS(BlueprintType)
class RIGEL_API ABillboardCluster : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABillboardCluster();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable)
    void  InitialPartiton(FVector2D LeftTop, FVector2D RightTopint, int Dimension);
	
#if WITH_EDITOR
    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
private:
    void CreateBillboard();

    //根据经纬度计算在哪一个分区
    void GetPartiton(const FVector2D& PoiLL, int XPartion, int YPartion);
public:
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FString GeoJsonPath;
	
    /** 材质引用 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rigel Billboard")
    UMaterialInterface* Material;

private:
    UInstancedStaticMeshComponent* ISMCComponent;
};
