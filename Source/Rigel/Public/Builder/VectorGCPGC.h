// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DynamicPCG.h"
#include "VectorSceneComponent.h"
#include "VectorGCPGC.generated.h"

struct FVectorFeatureData;

USTRUCT(BlueprintType)
struct FVectorPBGMap
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int64 Key = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPCGGraphInterface* InGraphInterface;
};


UCLASS()
class RIGEL_API AVectorGCPGC : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AVectorGCPGC();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostActorCreated();

	virtual void OnConstruction(const FTransform& Transform) override;
	
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

private:
	void CreatePCGActor();

	void SpawnPCG(const FVectorFeatureData& feature);

	void UpdatePCGProperty();
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Generate = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool CreateMesh = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnSpace = 3000.0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString VectorPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString MapField;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<int, FVectorPBGMap> FeildPCGDict;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UVectorSceneComponent* VectorComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<ADynamicPCG*> PCGActors;

	UPROPERTY(EditAnywhere)
	AGeoReferencingSystem* Georeference;
};
