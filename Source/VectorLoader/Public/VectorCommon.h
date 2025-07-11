// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VectorCommon.generated.h"

 USTRUCT(BlueprintType)
 struct FFeatureProperty
 {
     GENERATED_BODY()
 public:
     UPROPERTY(EditAnywhere, Category = "Mapmost", BlueprintReadWrite)
         FString Name = TEXT("");

     UPROPERTY(EditAnywhere, Category = "Mapmost", BlueprintReadWrite)
         FString Value;
 };

 USTRUCT(BlueprintType)
 struct FFeaturePropertyRow
 {
     GENERATED_BODY()
 public:
     UPROPERTY(EditAnywhere, Category = "Mapmost", BlueprintReadWrite)
     int FID = 0;

     UPROPERTY(EditAnywhere, Category = "Mapmost", BlueprintReadWrite)
     TMap<FString, FFeatureProperty> Properties;
 };

 USTRUCT(BlueprintType)
 struct FFeaturePropertyTable
 {
     GENERATED_BODY()
 public:
     //
    UPROPERTY(EditAnywhere, Category = "Mapmost", BlueprintReadWrite)
    TArray<FString> PropertyHeaders;

    //
    UPROPERTY(EditAnywhere, Category = "Mapmost", BlueprintReadWrite)
    TMap<int64, FFeaturePropertyRow> PropertiesMap;

 };

//

USTRUCT(BlueprintType)
struct FVectorGeometryData
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FVector> Points;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FVector> Normals;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> Indices;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FVector2D> UV0;
};

USTRUCT(BlueprintType)
struct FVectorFeatureData
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int64 FID = 0;
    //
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FVectorGeometryData> Geometrys;

    //
    TMap<FString, FJsonValue*> AttributeTable;
};

USTRUCT(BlueprintType)
struct FVectorLayerData
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Name;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FVectorFeatureData> Features;
};

USTRUCT(BlueprintType)
struct FVectorData
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FVectorLayerData> Layers;
};