// Fill out your copyright notice in the Description page of Project Settings.


#include "Builder/DynamicPCG.h"

#include "Iris/ReplicationState/ReplicationStateUtil.h"


// Sets default values
ADynamicPCG::ADynamicPCG()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PCGComponent = CreateDefaultSubobject<UPCGComponent>(TEXT("PCGComponent"));
	PCGComponent->bIsComponentPartitioned = false;
	PCGComponent->bGenerated = false;
}

// Called when the game starts or when spawned
void ADynamicPCG::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADynamicPCG::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADynamicPCG::UpdateMesh()
{
	Super::UpdateMesh();
	PCGLocation = GetActorLocation();
	PCGDirection = GetActorUpVector() * -1;
	if (Spline->GetNumberOfSplinePoints() > 3)
	{
		PCGComponent->Cleanup(false);
		PCGComponent->Generate(true);
	}
}

