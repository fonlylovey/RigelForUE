// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GeoJsonGeometry.h"
#include "GeoJsonProperties.h"
#include "GeoJsonFeature.generated.h"

/**
 * 
 */
USTRUCT()
struct RIGEL_API FGeoJsonFeature
{
	GENERATED_BODY()

    FString Name;

    EGeoJsonObjectType Type = EGeoJsonObjectType::Feature;

    FGeoJsonProperties* Properties = nullptr;

    FGeoJsonGeometry* Geometry = nullptr;
};
