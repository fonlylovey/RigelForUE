// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "QuadtreeSceneComponent.generated.h"

USTRUCT(BlueprintType)
struct FQuadtreeNode
{
    GENERATED_BODY()

    // 最小经度（西）、最大经度（东）
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quadtree")
    double MinLng = 106.0; // 示例：东经120°
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quadtree")
    double MaxLng = 107.0; // 示例：东经121°

    // 最小纬度（南）、最大纬度（北）
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quadtree")
    double MinLat = 28.0;  // 示例：北纬30°
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quadtree")
    double MaxLat = 30.0;  // 示例：北纬31°

    int Level = 1;

    FIntVector2 GirdIndex = FIntVector2(0, 0);

    // 检查某个经纬度是否在范围内
    bool Contains(const FVector& LatLng) const
    {
        // 约定：LatLng.X=纬度，LatLng.Y=经度，LatLng.Z=高度（暂不使用）
        return (LatLng.X >= MinLat && LatLng.X <= MaxLat)
            && (LatLng.Y >= MinLng && LatLng.Y <= MaxLng);
    }

    TArray<FQuadtreeNode*> ChildNodes;

    FQuadtreeNode* ParentNode = nullptr;


};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RIGEL_API UQuadtreeSceneComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UQuadtreeSceneComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UPROPERTY(EditAnywhere, Category = "Billboard Settings")
	int32 RootHeight = 100000;

    UPROPERTY(EditAnywhere, Category = "Billboard Settings")
    int32 MaxLevel = 10;

    int LevelInterval = 0;

    void OnPawnLocationChange(const FVector& GeoLocation);

};
