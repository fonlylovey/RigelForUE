#include "RigelPawn.h"

// Sets default values
ARigelPawn::ARigelPawn(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARigelPawn::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ARigelPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARigelPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

