#pragma once

#include "CoreMinimal.h"
#include <array>
#include <vector>

 /*
* https://github.com/mapbox/earcut.hpp
* 
*/

class VECTORLOADER_API VectorTool 
{
    using Ring = TArray<FVector>;
public:
	static TArray<int32> TriangulateLineString(const TArray<TArray<FVector3d>>& holes);
    
    static FVector projToGeo(OGRPoint* point, OGRCoordinateTransformation* trans)
    {
        double x = point->getX();
        double y = point->getY();
        double z = point->getZ();
        trans->Transform(1, &x, &y, &z);
        return FVector(x, y, z);
    }

    static void parsingPoint(FVectorFeatureData& feature, OGRPoint* point, OGRCoordinateTransformation* trans = nullptr)
    {
        FVectorGeometryData geom;
        if (trans != nullptr)
        {
            geom.Points.Add(projToGeo(point, trans));
        }
        else
        {
            geom.Points.Add(FVector3d(point->getX(), point->getY(), point->getZ()));
        }
        geom.Normals.Add(FVector(0, 0, 1));
        feature.Geometrys.Add(MoveTemp(geom));
    }

    static void parsingMultiPoint(FVectorFeatureData& feature, OGRMultiPoint* geoMultPoint, OGRCoordinateTransformation* trans = nullptr)
    {
        FVectorGeometryData geom;
        for (auto point : geoMultPoint)
        {
            if (trans != nullptr)
            {
                geom.Points.Add(projToGeo(point, trans));
            }
            else
            {
                geom.Points.Add(FVector3d(point->getX(), point->getY(), point->getZ()));
            }
            
            geom.Normals.Add(FVector(0, 0, 1));
        }
        
        feature.Geometrys.Add(MoveTemp(geom));
    }

    static void parsingLineString(FVectorFeatureData& feature, OGRLineString* geoLineString, OGRCoordinateTransformation* trans = nullptr)
    {
        FVectorGeometryData geom;
        for (auto& point : geoLineString)
        {
           if (trans != nullptr)
            {
                geom.Points.Add(projToGeo(point.toPoint(), trans));
            }
            else
            {
                geom.Points.Add(FVector(point.getX(), point.getY(), point.getZ()));
            }
            geom.Normals.Add(FVector(0, 0, 1));
        }
        feature.Geometrys.Add(MoveTemp(geom));
    }

    static void parsingMultiLineString(FVectorFeatureData& feature, OGRMultiLineString* geoMultLine, OGRCoordinateTransformation* trans = nullptr)
    {
        for (auto& lineString : geoMultLine)
        {
            FVectorGeometryData geom;
            for (auto& point : lineString)
            {
                if (trans != nullptr)
            {
                geom.Points.Add(projToGeo(point.toPoint(), trans));
            }
            else
            {
                geom.Points.Add(FVector(point.getX(), point.getY(), point.getZ()));
            }
                geom.Normals.Add(FVector(0, 0, 1));
            }
            feature.Geometrys.Add(MoveTemp(geom));
        }
        
    }

    static void parsingPolygon(FVectorFeatureData& feature, OGRPolygon* geoPoly, OGRCoordinateTransformation* trans = nullptr)
    {
        //
        //
        TArray<Ring> polyData;
        FVectorGeometryData geom;
        for (auto& ring : geoPoly)
        {
            Ring ringData;
            FVector thePoint;
            for (auto& point : ring)
            {
                if (trans != nullptr)
                {
                    thePoint = projToGeo(point.toPoint(), trans);
                }
                else
                {
                    thePoint = FVector(point.getX(), point.getY(), point.getZ());
                }
                geom.Points.Add(thePoint);
                geom.Normals.Add(FVector(0, 0, 1));
                ringData.Add(thePoint);
            }
            polyData.Add(MoveTemp(ringData));
            geom.Indices = VectorTool::TriangulateLineString(polyData);
        }
        feature.Geometrys.Add(MoveTemp(geom));
    }

    static void parsingMultiPolygon(FVectorFeatureData& feature, OGRMultiPolygon* geoMultPoly, OGRCoordinateTransformation* trans = nullptr)
    {
        //
        for (auto& poly : geoMultPoly)
        {
            TArray<Ring> polyData;
           //
           FVectorGeometryData geom;
           for (auto& ring : poly)
           {
               Ring ringData;
               for (auto& point : ring) 
               {
                   FVector thePoint = FVector3d(point.getX(), point.getY(), point.getZ());
                   if (trans != nullptr)
                   {
                       thePoint = projToGeo(point.toPoint(), trans);
                   }
                   else
                   {
                       thePoint = FVector(point.getX(), point.getY(), point.getZ());
                   }
                   geom.Points.Add(thePoint);
                   geom.Normals.Add(FVector(0, 0, 1));
                   ringData.Add(thePoint);
               }
               polyData.Add(ringData);
           }
           geom.Indices = VectorTool::TriangulateLineString(polyData);
           feature.Geometrys.Add(MoveTemp(geom));
        }
    }

};