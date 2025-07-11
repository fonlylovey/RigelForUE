// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VectorCommon.h"

class OGRPoint;
class OGRCoordinateTransformation;
/**
 * 
 */
class VECTORLOADER_API FGeojsonLoader
{
public:
    FVectorData* LoadFile(const FString& strFilePath);
    
private:    
};
