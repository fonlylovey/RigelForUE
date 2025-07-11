// Fill out your copyright notice in the Description page of Project Settings.


#include "VectorSceneComponent.h"
#include "VectorMeshComponent.h"
#include "VectorCommon.h"
#include "GeojsonLoader.h"
#include "SHPLoader.h"

UVectorSceneComponent::UVectorSceneComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them./Script/Engine.Material
	PrimaryComponentTick.bCanEverTick = true;

	
    FString str = TEXT("/VectorLoader/Materials/M_VectorBase.M_VectorBase");
    BaseMaterial = Cast<UMaterial>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, *str));
}


// Called when the game starts
void UVectorSceneComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UVectorSceneComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}



void UVectorSceneComponent::LoadVector(const FString& FilePath)
{
    DestoryVector();
    VectorPath = FilePath;
    FVectorData* pVector = nullptr;
    if (!VectorPath.IsEmpty())
    {
        FString fileSuffix = FPaths::GetExtension(VectorPath);
	    
        if (fileSuffix.Equals("shp", ESearchCase::IgnoreCase))
        {
            TSharedPtr<FSHPLoader> reader = MakeShared<FSHPLoader>();
            pVector = reader->LoadFile(VectorPath);
        }
        else if(fileSuffix.Equals("geojson", ESearchCase::IgnoreCase))
        {
            TSharedPtr<FGeojsonLoader> reader = MakeShared<FGeojsonLoader>();
           pVector = reader->LoadFile(VectorPath);
        }
    }
    if (pVector != nullptr && IsCreateMesh)
    {
        CreateMesh(pVector);
    }
}

FVectorData* UVectorSceneComponent::LoadData(const FString& FilePath)
{
    VectorPath = FilePath;
    FVectorData* pVector = nullptr;
    if (!FilePath.IsEmpty())
    {
        FString fileSuffix = FPaths::GetExtension(VectorPath);
	    
        if (fileSuffix.Equals("shp", ESearchCase::IgnoreCase))
        {
            TSharedPtr<FSHPLoader> reader = MakeShared<FSHPLoader>();
            pVector = reader->LoadFile(VectorPath);
            reader.Reset();
        }
        else if(fileSuffix.Equals("geojson", ESearchCase::IgnoreCase))
        {
            TSharedPtr<FGeojsonLoader> reader = MakeShared<FGeojsonLoader>();
            pVector = reader->LoadFile(VectorPath);
        }
    }
    return pVector;
}

void UVectorSceneComponent::DestoryVector()
{
    for (UVectorMeshComponent* vector : LayerMeshes)
    {
        if (IsValid(vector))
        {
            vector->RemoveFromRoot();
            vector->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
            vector->ConditionalBeginDestroy();
            vector = nullptr;
        }
    }
    LayerMeshes.Empty(0);
}

void UVectorSceneComponent::OnComponentCreated()
{
    Super::OnComponentCreated();
}

void UVectorSceneComponent::OnRegister()
{
    Super::OnRegister();
    if (Georeference == nullptr)
    {
        Georeference = AGeoReferencingSystem::GetGeoReferencingSystem(this->GetOwner());
    }
    LoadVector(VectorPath);
}

void UVectorSceneComponent::Activate(bool bReset/*=false*/)
{
    Super::Activate(bReset);
}

void UVectorSceneComponent::CreateMesh(FVectorData* pVector)
{
     for (const auto& layer : pVector->Layers)
     {
         UVectorMeshComponent* layerMesh = NewObject<UVectorMeshComponent>(this, *layer.Name);
         layerMesh->SetUsingAbsoluteLocation(true);
         layerMesh->SetMobility(EComponentMobility::Static);
         layerMesh->SetFlags(RF_Transient | RF_DuplicateTransient | RF_TextExportTransient);
         
         layerMesh->CastShadow = true;
         layerMesh->Material = BaseMaterial;
         layerMesh->SetMaterialByName(*layer.Name, BaseMaterial);
         layerMesh->CreateMesh(layer, Georeference);
         layerMesh->bEditableWhenInherited = true;
         layerMesh->SetupAttachment(this);
         layerMesh->RegisterComponent();
         layerMesh->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
         LayerMeshes.Add(layerMesh);
     }
}

