// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GeoJson/GeoJsonFeature.h"
#include "GeoJsonFeatureCollection.generated.h"

/**
 * 
 */
USTRUCT()
struct RIGEL_API FGeoJsonFeatureCollection
{
	GENERATED_BODY()
	
	FString Name;

	EGeoJsonObjectType Type = EGeoJsonObjectType::FeatureCollection;

    FString CRS;

    TArray<FGeoJsonFeature*> Features;

};
