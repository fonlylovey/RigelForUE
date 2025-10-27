// Fill out your copyright notice in the Description page of Project Settings.


#include "Geojson/GeoJsonGeometry.h"

FGeoJsonPoint* FGeoJsonGeometry::AsPoint()
{
    FGeoJsonPoint* vPtr = (FGeoJsonPoint*)(this);
    return vPtr;
}
