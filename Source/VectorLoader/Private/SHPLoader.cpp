// Fill out your copyright notice in the Description page of Project Settings.


#include "SHPLoader.h"
#include "gdal_priv.h"
#include "ogrsf_frmts.h"
#include "gdal.h"
#include "ogr_spatialref.h"
#include "VectorTool.h"

FVectorData* FSHPLoader::LoadFile(const FString& strFilePath)
{
    GDALAllRegister();

    GDALDataset* pDataSet = nullptr;
    GDALDriver* pDriver = nullptr;
    pDataSet = (GDALDataset*) GDALOpenEx(TCHAR_TO_UTF8(*strFilePath), GDAL_OF_VECTOR, nullptr, nullptr, nullptr);
    if (pDataSet == nullptr)
    {
        return nullptr;
    }
    FVectorData* pData = new FVectorData;
    for ( OGRLayer* const &layer : pDataSet->GetLayers())
    {
        const OGRSpatialReference* CRS = layer->GetSpatialRef();
        OGRCoordinateTransformation* projToGeoTrans = nullptr;
        if (CRS->IsProjected() == 1)
        {
           projToGeoTrans = OGRCreateCoordinateTransformation(CRS, CRS->CloneGeogCS());
        }
       
        FVectorLayerData layerData;
        layerData.Name = layer->GetName();
       
        for (auto& feature : layer)
        {
            FVectorFeatureData featureData;
            featureData.FID = feature->GetFID();
            OGRFeatureDefn* layerDefn = layer->GetLayerDefn();
           
            for (int i = 0; i < layerDefn->GetFieldCount(); i++)
            {
                OGRFieldDefn* fieldDefn = layerDefn->GetFieldDefn(i);
                std::string strName(fieldDefn->GetNameRef());
                FString strFiledName = fieldDefn->GetNameRef();
                OGRFieldType fieldType = fieldDefn->GetType();
                const OGRField* fieldPtr = feature->GetRawFieldRef(i);
                //FJsonObject* fieldObj = new FJsonObject;
                FJsonValue* valuePtr = nullptr;
                if (fieldType == OFTInteger)
                {
                    //int value = feature->GetFieldAsIntegerUnsafe(i);
                    int value = fieldPtr->Integer;
                    valuePtr = new FJsonValueNumber(value);
                    //fieldObj->SetNumberField(strFiledName, value);
                    featureData.AttributeTable.Add(strFiledName, valuePtr);
                }
                else if (fieldType == OFTInteger64)
                {
                    int64 value = fieldPtr->Integer64;
                    valuePtr = new FJsonValueNumber(value);
                    //fieldObj->SetNumberField(strFiledName, value);
                    featureData.AttributeTable.Add(strFiledName, valuePtr);
                }
                else if (fieldType == OFTReal)
                {
                    double value = fieldPtr->Real;
                    valuePtr = new FJsonValueNumber(value);
                    //fieldObj->SetNumberField(strFiledName, value);
                    featureData.AttributeTable.Add(strFiledName, valuePtr);
                }
                else if(fieldType == OFTString)
                {
                    FString value = fieldPtr->String;
                    valuePtr = new FJsonValueString(value);
                    //fieldObj->SetStringField(strFiledName, value);
                    featureData.AttributeTable.Add(strFiledName, valuePtr);
                }
            }
            
            //¶ÁÈ¡¶¥µãÖµ
            OGRGeometry* pGeometry = feature->GetGeometryRef();
            if (pGeometry != nullptr)
            {
                 OGRwkbGeometryType geomType = pGeometry->getGeometryType();
                switch (geomType)
                {
                case wkbPoint:
                {
                    OGRPoint* geoPoint = pGeometry->toPoint();
                    VectorTool::parsingPoint(featureData, geoPoint, projToGeoTrans);
                }
                break;
                case wkbLineString:
                {
                    OGRLineString* geoLineString = pGeometry->toLinearRing();
                    VectorTool::parsingLineString(featureData, geoLineString, projToGeoTrans);
                }
                break;
                case wkbPolygon:
                {
                    OGRPolygon* geoPoly = pGeometry->toPolygon();
                    VectorTool::parsingPolygon(featureData, geoPoly, projToGeoTrans);
                }
                break;
                case wkbMultiPoint:
                {
                    OGRMultiPoint* geoMultPoint = pGeometry->toMultiPoint();
                    VectorTool::parsingMultiPoint(featureData, geoMultPoint, projToGeoTrans);
                }
                break;
                case wkbMultiLineString:
                {
                    OGRMultiLineString* geoMultLine = pGeometry->toMultiLineString();
                    VectorTool::parsingMultiLineString(featureData, geoMultLine, projToGeoTrans);
                }
                break;
                case wkbMultiPolygon:
                {
                    OGRMultiPolygon* geoMultPoly = pGeometry->toMultiPolygon();
                    VectorTool::parsingMultiPolygon(featureData, geoMultPoly, projToGeoTrans);
                }
                break;
                default:
                    break;
                }
                layerData.Features.Add(MoveTemp(featureData));
            }
           
        }
        pData->Layers.Add(MoveTemp(layerData));
    }
    GDALClose(pDataSet);
    return pData;
}
