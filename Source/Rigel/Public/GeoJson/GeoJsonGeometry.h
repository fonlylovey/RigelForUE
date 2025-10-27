// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GeoJsonGeometry.generated.h"


UENUM(BlueprintType)
enum class EGeoJsonObjectType : uint8
{
    FeatureCollection,
    Feature,
    Point,
    MultiPoint,
    LineString,
    MultiLineString,
    Polygon,
    MultiPolygon,
    GeometryCollection,
};


struct FGeoJsonPoint;
USTRUCT(BlueprintType)
struct RIGEL_API FGeoJsonGeometry
{
    GENERATED_BODY()
      
public:
    FGeoJsonGeometry() {

    }

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GeoJson")
    EGeoJsonObjectType Type = EGeoJsonObjectType::Point;

    
    FGeoJsonPoint* AsPoint();
};

USTRUCT(BlueprintType)
struct RIGEL_API FGeoJsonPoint : public FGeoJsonGeometry
{
    GENERATED_BODY()

public:
    FGeoJsonPoint() {

    }

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GeoJson")
    FVector Point;
};

USTRUCT(BlueprintType)
struct RIGEL_API FGeoJsonRing : public FGeoJsonGeometry
{
    GENERATED_BODY()

public:
    FGeoJsonRing() {

    }

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GeoJson")
    TArray<FVector> Coordinates;
};

/**
 * 
 */
USTRUCT(BlueprintType)
struct RIGEL_API FGeoJsonMultiGeometry
{
	GENERATED_BODY()

public:
    FGeoJsonMultiGeometry() {

    }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GeoJson")
    TArray<FGeoJsonRing> Coordinates;
};

