// Fill out your copyright notice in the Description page of Project Settings.


#include "VectorGCPGC.h"

#include "VectorCommon.h"


// Sets default values
AVectorGCPGC::AVectorGCPGC()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	VectorComponent = CreateDefaultSubobject<UVectorSceneComponent>(TEXT("VectorComponent"));
}

// Called when the game starts or when spawned
void AVectorGCPGC::BeginPlay()
{
	Super::BeginPlay();
    //CreatePCGActor();
	
}

void AVectorGCPGC::PostActorCreated()
{
	Super::PostActorCreated();
}

void AVectorGCPGC::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, " AVectorGCPGC::OnConstruction()");
}

#if WITH_EDITOR
void AVectorGCPGC::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	FName propertyName = PropertyChangedEvent.Property->GetFName();
	
	if (propertyName ==
		GET_MEMBER_NAME_CHECKED(AVectorGCPGC, VectorPath) ||
		propertyName ==
		GET_MEMBER_NAME_CHECKED(AVectorGCPGC, CreateMesh) ||
		propertyName ==
		GET_MEMBER_NAME_CHECKED(AVectorGCPGC, Generate) ||
		propertyName ==
		GET_MEMBER_NAME_CHECKED(AVectorGCPGC, SpawnSpace)) 
	{
		CreatePCGActor();
		UpdatePCGProperty();
	}
	Super::PostEditChangeProperty(PropertyChangedEvent);
}

#endif

void AVectorGCPGC::CreatePCGActor()
{
	
	if (Georeference == nullptr)
	{
		Georeference = AGeoReferencingSystem::GetGeoReferencingSystem(this);
	}
	
	VectorComponent->IsCreateMesh = CreateMesh;
	if (Generate)
	{
		for (auto actor : PCGActors)
		{
			GetWorld()->DestroyActor(actor);
		}
		PCGActors.Empty();
		FVectorData* VectorData = VectorComponent->LoadData(VectorPath);
		if (VectorData != nullptr)
		{
			for (auto layer : VectorData->Layers)
			{
				for (auto feature : layer.Features)
				{
					SpawnPCG(feature);
				}
			}
		}
	}
	else
	{
		for (auto actor : PCGActors)
		{
			actor->UpdateMesh();
			
		}
	}
	
}

void AVectorGCPGC::SpawnPCG(const FVectorFeatureData& feature)
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	FTransform trans;
	auto fieldPtr = feature.AttributeTable.FindRef(MapField);

	int fieldValue = 0;
	if (fieldPtr != nullptr)
	{
		fieldPtr->TryGetNumber(fieldValue);
	}

	if (FeildPCGDict.Contains(fieldValue))
	{
		AActor* actor = GWorld->SpawnActor(ADynamicPCG::StaticClass(), &trans, SpawnInfo);
		ADynamicPCG* pcgActor = Cast<ADynamicPCG>(actor);
		auto pcgGraph = FeildPCGDict[fieldValue];
		pcgActor->PCGComponent->SetGraph(pcgGraph.InGraphInterface);
		pcgActor->Spline->ClearSplinePoints();
		pcgActor->SpawnSpace = SpawnSpace;
		pcgActor->SetActorLocation(FVector(0, 0, 1000000));
		PCGActors.Add(pcgActor);
		for (auto geom : feature.Geometrys)
		{
			int32 index = 0; 
			for (FVector point : geom.Points)
			{
				if (Georeference != nullptr)
				{
					FVector ueLocation;
					Georeference->GeographicToEngine(FGeographicCoordinates(point.X, point.Y, point.Z + Georeference->OriginAltitude), ueLocation);
					pcgActor->Spline->AddSplineWorldPoint(ueLocation);
					pcgActor->Spline->SetSplinePointType(index, ESplinePointType::Linear);
					index++;
				}
			}
		}
		pcgActor->Spline->UpdateSpline();
		pcgActor->UpdateMesh();
	}
}

void AVectorGCPGC::UpdatePCGProperty()
{
	for (auto actor : PCGActors)
	{
		actor->SpawnSpace = SpawnSpace;
	}
}

// Called every frame
void AVectorGCPGC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
