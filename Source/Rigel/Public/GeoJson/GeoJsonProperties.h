// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GeoJsonProperties.generated.h"

/**
 * 
 */
USTRUCT()
struct RIGEL_API FGeoJsonProperties
{
	GENERATED_BODY()

public:
    FGeoJsonProperties() {

    }

    TMap<FString, FString> Properties;
};
