// Fill out your copyright notice in the Description page of Project Settings.


#include "VectorMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine/Polys.h"
#include "PhysicsEngine/ConvexElem.h"
#include "PhysicsEngine/BodySetup.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "GeographicCoordinates.h"
#include "GeoReferencingSystem.h"

#define RCP_SQRT2 (0.70710678118654752440084436210485f)
#define RCP_SQRT3 (0.57735026918962576450914878050196f)

const FVector KDopDir26[26] =
{
	FVector( 1.f, 0.f, 0.f),
	FVector(-1.f, 0.f, 0.f),
	FVector( 0.f, 1.f, 0.f),
	FVector( 0.f,-1.f, 0.f),
	FVector( 0.f, 0.f, 1.f),
	FVector( 0.f, 0.f,-1.f),
	FVector( 0.f, RCP_SQRT2,  RCP_SQRT2),
	FVector( 0.f,-RCP_SQRT2, -RCP_SQRT2),
	FVector( 0.f, RCP_SQRT2, -RCP_SQRT2),
	FVector( 0.f,-RCP_SQRT2,  RCP_SQRT2),
	FVector( RCP_SQRT2, 0.f,  RCP_SQRT2),
	FVector(-RCP_SQRT2, 0.f, -RCP_SQRT2),
	FVector( RCP_SQRT2, 0.f, -RCP_SQRT2),
	FVector(-RCP_SQRT2, 0.f,  RCP_SQRT2),
	FVector( RCP_SQRT2,  RCP_SQRT2, 0.f),
	FVector(-RCP_SQRT2, -RCP_SQRT2, 0.f),
	FVector( RCP_SQRT2, -RCP_SQRT2, 0.f),
	FVector(-RCP_SQRT2,  RCP_SQRT2, 0.f),
	FVector( RCP_SQRT3,  RCP_SQRT3,  RCP_SQRT3),
	FVector( RCP_SQRT3,  RCP_SQRT3, -RCP_SQRT3),
	FVector( RCP_SQRT3, -RCP_SQRT3,  RCP_SQRT3),
	FVector( RCP_SQRT3, -RCP_SQRT3, -RCP_SQRT3),
	FVector(-RCP_SQRT3,  RCP_SQRT3,  RCP_SQRT3),
	FVector(-RCP_SQRT3,  RCP_SQRT3, -RCP_SQRT3),
	FVector(-RCP_SQRT3, -RCP_SQRT3,  RCP_SQRT3),
	FVector(-RCP_SQRT3, -RCP_SQRT3, -RCP_SQRT3),
};

constexpr float HalfWorldMax = HALF_WORLD_MAX;

UVectorMeshComponent::UVectorMeshComponent()
{
    
}

UVectorMeshComponent::~UVectorMeshComponent()
{

}

void UVectorMeshComponent::BeginDestroy()
{
    Super::BeginDestroy();
}


void UVectorMeshComponent::CreateMesh(const FVectorLayerData& layer, AGeoReferencingSystem* georeference)
{
	LayerName = *layer.Name;
    UStaticMesh* pStaticMesh = NewObject<UStaticMesh>(this, *layer.Name);
    SetStaticMesh(pStaticMesh);
    pStaticMesh->NeverStream = true;
    pStaticMesh->SetFlags(RF_Transient | RF_DuplicateTransient | RF_TextExportTransient);
    TUniquePtr<FStaticMeshRenderData> RenderData = MakeUnique<FStaticMeshRenderData>();
    RenderData->AllocateLODResources(1);
    pStaticMesh->CreateBodySetup();
    FStaticMeshLODResources& LODResources = RenderData->LODResources[0];
    LODResources.bHasColorVertexData = false;
    LODResources.bHasDepthOnlyIndices = false;
    LODResources.bHasReversedIndices = false;
    LODResources.bHasReversedDepthOnlyIndices = false;
    
    //绘制面
    FBox3d box;
    box.Init();

    int32 sectionIndex = 0;
    uint32 IndexFlag = 0;
    TArray<FStaticMeshBuildVertex> vertexData;

    FStaticMeshSectionArray& sectionArray = LODResources.Sections;
    for (const auto& feature : layer.Features)
    {
    	FName fName = FName( FString::Printf(TEXT("%llu"), feature.FID));
    	auto matInst = UMaterialInstanceDynamic::Create(Material, this, fName);
    	FLinearColor color = FLinearColor(FMath::RandRange(0.f, 1.f), FMath::RandRange(0.f, 1.f), FMath::RandRange(0.f, 1.f));
    	matInst->SetVectorParameterValue("FillColor", color);
    	pStaticMesh->AddMaterial(matInst);
        for (const auto& geom : feature.Geometrys)
        {
            int32 count = geom.Indices.Num();
            FStaticMeshSection& staticSection = sectionArray.AddDefaulted_GetRef();
            staticSection.bEnableCollision = true;
            staticSection.MaterialIndex = 0;
            staticSection.NumTriangles = count / 3;
            staticSection.FirstIndex = sectionIndex;
            staticSection.MinVertexIndex = sectionIndex;
            staticSection.MaxVertexIndex = sectionIndex + count - 1;
            for (int32 i = 0; i < geom.Points.Num(); i++)
            {
                FStaticMeshBuildVertex vertex;
                FVector3f unPos = FVector3f(geom.Points[i]);
                if (georeference != nullptr)
                {
                    FVector engpos;
                    georeference->GeographicToEngine(FGeographicCoordinates(unPos.X, unPos.Y, unPos.Z + georeference->OriginAltitude), engpos);
                    unPos = FVector3f(engpos);
                }
                vertex.Position = unPos;
                vertex.Color = FColor::Red;
                vertex.UVs[0] = FVector2f(0, 0);
                vertex.TangentX = FVector3f(1, 0, 0);
                vertex.TangentY = FVector3f(0, 1, 0);
                vertex.TangentZ = FVector3f(0, 0, 1);
                box += FVector(vertex.Position);
                vertexData.Add(vertex);
            }

            TArray<uint32> indices;
            for (int32 i = 0; i < geom.Indices.Num(); i++)
            {
                indices.Add( geom.Indices[i] + IndexFlag);
            }

            LODResources.IndexBuffer.AppendIndices(indices.GetData(), indices.Num());
            sectionIndex = sectionIndex + count;
            IndexFlag = vertexData.Num();
        }
    }
    
    LODResources.VertexBuffers.PositionVertexBuffer.AppendVertices(vertexData.GetData(), vertexData.Num());
	LODResources.VertexBuffers.StaticMeshVertexBuffer.AppendVertices(vertexData.GetData(), vertexData.Num());
    
    //设置包围盒
    FBoxSphereBounds BoundingBoxAndSphere;
    BoundingBoxAndSphere.Origin = FVector(box.GetCenter());
    BoundingBoxAndSphere.BoxExtent = FVector(box.GetExtent());
    BoundingBoxAndSphere.SphereRadius = BoundingBoxAndSphere.BoxExtent.Size();
    RenderData->Bounds = MoveTemp(BoundingBoxAndSphere);
    pStaticMesh->SetRenderData(MoveTemp(RenderData));

    pStaticMesh->InitResources();
    pStaticMesh->CalculateExtendedBounds();
}

void UVectorMeshComponent::AddMeshSection(const TArray<FStaticMeshBuildVertex>& vertexes)
{

}

void UVectorMeshComponent::CreateCollision()
{
    float DirectionCount = 26;
    TArray<float> MaxDistances;
	MaxDistances.Reserve(DirectionCount);

	for (int32 Index = 0; Index < DirectionCount; ++Index)
	{
		MaxDistances.Add(-MAX_FLT);
	}
    if (GetStaticMesh() == nullptr || GetStaticMesh()->GetRenderData() == nullptr)
    {
        return;
    }

    FStaticMeshLODResources& LODResources = GetStaticMesh()->GetRenderData()->LODResources[0];

	// For each vertex, project along each kdop direction, to find the max in that direction.
	const FPositionVertexBuffer& PositionVertexBuffer = LODResources.VertexBuffers.PositionVertexBuffer;
	for(int32 Index = 0; Index < LODResources.GetNumVertices(); ++Index)
	{
		for(int32 DirIndex = 0; DirIndex < DirectionCount; ++DirIndex)
		{
			const float Dist = (FVector)PositionVertexBuffer.VertexPosition(Index) | KDopDir26[DirIndex];
			MaxDistances[DirIndex] = FMath::Max(Dist, MaxDistances[DirIndex]);
		}
	}

	// Inflate MaxDistances to ensure it is no degenerate
	const float MinSize = 0.1f;
	for (int32 Index = 0; Index < DirectionCount; ++Index)
	{
		MaxDistances[Index] += MinSize;
	}

	// Now we have the Planes of the kdop, we work out the face polygons.
	TArray<FPlane> Planes;
	Planes.Reserve(DirectionCount);
	for (int32 Index = 0; Index < DirectionCount; ++Index)
	{
		Planes.Add( FPlane(KDopDir26[Index], MaxDistances[Index]) );
	}

	TArray<FPoly> Element;
	Element.Reserve(DirectionCount);
	for (int32 Index = 0; Index < DirectionCount; ++Index)
	{
		FPoly&	Polygon = Element.AddZeroed_GetRef();
		FVector3f Base, AxisX, AxisY;

		Polygon.Init();
		Polygon.Normal = (FVector3f)Planes[Index];
		Polygon.Normal.FindBestAxisVectors(AxisX, AxisY);

		Base = FVector3f(Planes[Index] * Planes[Index].W);

		Polygon.Vertices.Reserve(4);
		new(Polygon.Vertices) FVector3f(Base + AxisX * HalfWorldMax + AxisY * HalfWorldMax);
		new(Polygon.Vertices) FVector3f(Base + AxisX * HalfWorldMax - AxisY * HalfWorldMax);
		new(Polygon.Vertices) FVector3f(Base - AxisX * HalfWorldMax - AxisY * HalfWorldMax);
		new(Polygon.Vertices) FVector3f(Base - AxisX * HalfWorldMax + AxisY * HalfWorldMax);

		for (int32 Jndex = 0; Jndex < DirectionCount; ++Jndex)
		{
			if(Index != Jndex)
			{
				if(!Polygon.Split(-FVector3f(Planes[Jndex]), FVector3f(Planes[Jndex] * Planes[Jndex].W)))
				{
					Polygon.Vertices.Empty();
					break;
				}
			}
		}

		if(Polygon.Vertices.Num() < 3)
		{
			// If poly resulted in no verts, remove from array
			Element.Pop(false);
		}
		else
		{
			// Other stuff...
			Polygon.iLink = Index;
			Polygon.CalcNormal(1);
		}
	}

	if(Element.Num() < 4)
	{
		return;
	}

	FKConvexElem& ConvexElem = GetStaticMesh()->GetBodySetup()->AggGeom.ConvexElems.AddDefaulted_GetRef();

	for (FPoly& Poly : Element)
	{
		for (FVector3f& Position : Poly.Vertices)
		{
			ConvexElem.VertexData.Add((FVector)Position);
		}
	}

	ConvexElem.UpdateElemBox();
}

 