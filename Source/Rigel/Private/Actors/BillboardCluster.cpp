// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/BillboardCluster.h"
#include "Kismet/GameplayStatics.h"
#include "ToolLibrary/RigelGeoFunctionLibrary.h"
#include "GeoJson/GeoJsonFeatureCollection.h"
#include "Actors/POIBase.h"
#include "CesiumGeoreference.h"


// Sets default values
ABillboardCluster::ABillboardCluster()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABillboardCluster::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABillboardCluster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
   
	
}

void ABillboardCluster::InitialPartiton(FVector2D LeftTop, FVector2D RightTopint, int Dimension)
{

}

static FVector2D GetCenterByPoints(TArray<FVector2D> Locations)
{
	FVector2D SumLocation;
	for (auto Location:Locations)
	{
		SumLocation=SumLocation+Location;
	}

	return  SumLocation/Locations.Num();
}

void ABillboardCluster::GetPartiton(const FVector2D& PoiLL, int XPartion, int YPartion)
{

}

#if WITH_EDITOR
void ABillboardCluster::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);
    if (!PropertyChangedEvent.Property)
    {
        return;
    }

    FName propertyName = PropertyChangedEvent.Property->GetFName();

    if (propertyName == GET_MEMBER_NAME_CHECKED(ABillboardCluster, GeoJsonPath))
    {
        CreateBillboard();
    }
    else if (propertyName == GET_MEMBER_NAME_CHECKED(ABillboardCluster, Material))
    {

    }
}
#endif

void ABillboardCluster::CreateBillboard()
{
    int32 count = 0;
    FGeoJsonFeatureCollection collection;
    bool isOK = URigelGeoFunctionLibrary::ReadGeojsonByFile(GeoJsonPath, collection);
    for (FGeoJsonFeature* feature : collection.Features)
    {
        URigelBillboardComponent* Component = NewObject<URigelBillboardComponent>(this, NAME_None, RF_Transient);
        if (Material != nullptr)
        {
            //Component->SetMaterial(0, Material);
            //Component->CreateMaterial();
        }
        ACesiumGeoreference* Georeference = ACesiumGeoreference::GetDefaultGeoreference(GetWorld(
        ));
        if (Georeference != nullptr && feature->Geometry != nullptr)
        {
            FGeoJsonPoint* point = feature->Geometry->AsPoint();
            if (point != nullptr)
            {
                FVector llh = point->Point;
                FVector unrealLocation = Georeference->TransformLongitudeLatitudeHeightPositionToUnreal(llh);
                Component->SetWorldLocation(unrealLocation);
                AddInstanceComponent(Component);
                count++;
            }
        }
        if (count > 100)
        {
            break;
        }
    }
}
