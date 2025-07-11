// Fill out your copyright notice in the Description page of Project Settings.


#include "VectorFunctionLibrary.h"
#include "SHPLoader.h"
#include "GeojsonLoader.h"

FVectorData UVectorFunctionLibrary::LoadVectorFile(const FString& FilePath)
{
	FVectorData* pVectorData = nullptr;
	if (!FilePath.IsEmpty())
	{
		FString fileSuffix = FPaths::GetExtension(FilePath);
	    
		if (fileSuffix.Equals("shp", ESearchCase::IgnoreCase))
		{
			TSharedPtr<FSHPLoader> reader = MakeShared<FSHPLoader>();
			pVectorData = reader->LoadFile(FilePath);
		}
		else if(fileSuffix.Equals("geojson", ESearchCase::IgnoreCase))
		{
			TSharedPtr<FGeojsonLoader> reader = MakeShared<FGeojsonLoader>();
			pVectorData = reader->LoadFile(FilePath);
		}
	}
	if (pVectorData != nullptr)
	{
		return *pVectorData;
	}
	return FVectorData();
}
