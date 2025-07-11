// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "GeoReferencingSystem.h"
#include "VectorSceneComponent.generated.h"

struct FVectorData;
class UVectorMeshComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VECTORLOADER_API UVectorSceneComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UVectorSceneComponent();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//
	UFUNCTION(BlueprintCallable)
    void LoadVector(const FString& FilePath);

	//
	FVectorData* LoadData(const FString& FilePath);
	
    void DestoryVector();

    virtual void OnComponentCreated();

    virtual void OnRegister();

    virtual void Activate(bool bReset=false);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
    void CreateMesh(FVectorData* pVector);
	
    UMaterial* BaseMaterial = nullptr;

public:
    UPROPERTY(EditAnywhere)
	FString VectorPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsCreateMesh = true;
	
    UPROPERTY(EditAnywhere)
    TArray<UVectorMeshComponent*> LayerMeshes;

    // UPROPERTY(EditAnywhere)
    //TMap<FString, UMaterial*> Legendes;

    UPROPERTY(EditAnywhere)
    AGeoReferencingSystem* Georeference;
};
