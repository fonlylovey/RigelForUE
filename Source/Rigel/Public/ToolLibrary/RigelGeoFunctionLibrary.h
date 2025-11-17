// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GeoJson/GeoJsonFeatureCollection.h"
#include "RigelGeoFunctionLibrary.generated.h"

class AGeoReferencingSystem;
class UStaticMeshComponent;

/**
 * 
 */
UCLASS(BlueprintType, meta = (ScriptName = "Rigel"))
class RIGEL_API URigelGeoFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * 
	 * @param GeoReferencingSystem 
	 * @param StaticMeshComponent 
	 * @param Offset Length unit is m.
	 */
	UFUNCTION(BlueprintCallable, Category="Rigel")
	static void ProjectMeshesToEngine(AGeoReferencingSystem* GeoReferencingSystem, UStaticMeshComponent* StaticMeshComponent, const FVector Offset);

    static bool ReadGeojsonByFile(const FString& FilePath, FGeoJsonFeatureCollection& OutData);

    static void ReadGeojsonByString(const FString& JsonData, FGeoJsonFeatureCollection& OutData);
};
