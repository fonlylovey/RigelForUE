// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AbcimGeoFunctionLibrary.generated.h"

class AGeoReferencingSystem;
class UStaticMeshComponent;

/**
 * 
 */
UCLASS(BlueprintType, meta = (ScriptName = "AbcimGeoLibrary"))
class ABCIMTOOLEDITOR_API UAbcimGeoFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * 
	 * @param GeoReferencingSystem 
	 * @param StaticMeshComponent 
	 * @param Offset Length unit is m.
	 */
	UFUNCTION(BlueprintCallable, Category="Abcim")
	static void ProjectMeshesToEngine(AGeoReferencingSystem* GeoReferencingSystem, UStaticMeshComponent* StaticMeshComponent, const FVector Offset);
};
