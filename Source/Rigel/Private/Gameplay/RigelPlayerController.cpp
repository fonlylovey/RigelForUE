#include "Gameplay/RigelPlayerController.h"
#include "WebAPI/RigelAPISubsystem.h"

ARigelPlayerController::ARigelPlayerController(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    this->bEnableClickEvents = true;
}

void ARigelPlayerController::OnMessageByWeb(const FString& Data)
{
    URigelAPISubsystem::RigelAPISubsystem()->Invoke(Data);
}

void ARigelPlayerController::BeginPlay()
{
    Super::BeginPlay();
}

// PlayerController no longer binds Enhanced Input; RigelPawn handles its own input mapping and bindings.
