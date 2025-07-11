// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/StaticMeshComponent.h"
#include "VectorCommon.h"
#include "ProceduralMeshComponent.h"
#include "VectorMeshComponent.generated.h"

class AGeoReferencingSystem;

/**
 * 
 */
UCLASS(meta=(BlueprintSpawnableComponent))
class UVectorMeshComponent : public UStaticMeshComponent
{
	GENERATED_BODY()
	
public:
    UVectorMeshComponent();
    virtual ~UVectorMeshComponent();

    virtual void BeginDestroy() override;

    //每张瓦片的图层中的所有要素创建一个组件
    virtual void CreateMesh(const FVectorLayerData& layer, AGeoReferencingSystem* georeference);

    void AddMeshSection(const TArray<FStaticMeshBuildVertex>& vertexes);

    void CreateCollision();

public:
    UPROPERTY(EditAnywhere)
	UMaterialInterface* Material = nullptr;

    UPROPERTY(VisibleAnywhere)
    FString LayerName = TEXT("");

    FFeaturePropertyRow FeatureProperty;
};
