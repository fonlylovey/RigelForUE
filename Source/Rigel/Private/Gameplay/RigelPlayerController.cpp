#include "Gameplay/RigelPlayerController.h"
#include "WebAPI/RigelAPISubsystem.h"

ARigelPlayerController::ARigelPlayerController(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    PixelStreamingInput = CreateDefaultSubobject<UPixelStreamingInput>(TEXT("PixelStreamingInput"));
    PixelStreamingInput->OnInputEvent.AddDynamic(this, &ARigelPlayerController::OnMessageByWeb);
    this->bEnableClickEvents = true;
}

void ARigelPlayerController::OnMessageByWeb(const FString& Data)
{
    //{Name:SetWeather,"Param": "Sunny"}
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, Data);
    URigelAPISubsystem::RigelAPISubsystem()->Invoke(Data);
}

void ARigelPlayerController::SendMessageToWeb(const FString& Data)
{
    if (PixelStreamingInput != nullptr)
    {
        PixelStreamingInput->SendPixelStreamingResponse(Data);
    }
}

void ARigelPlayerController::BeginPlay()
{
    Super::BeginPlay();
}
