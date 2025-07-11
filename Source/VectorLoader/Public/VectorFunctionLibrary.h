// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VectorCommon.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "VectorFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class VECTORLOADER_API UVectorFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
    UFUNCTION(BlueprintCallable, Category = "VectorLoader")
    static FVectorData LoadVectorFile(const FString& FilePath);
};
