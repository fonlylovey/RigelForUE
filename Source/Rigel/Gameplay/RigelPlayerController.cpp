#include "RigelPlayerController.h"
#include "RigelAPIManager.h"


ARigelPlayerController::ARigelPlayerController(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    PixelStreamingInput = CreateDefaultSubobject<UPixelStreamingInput>(TEXT("PixelStreamingInput"));
}

void ARigelPlayerController::OnMessageByWeb(const FString& Data)
{
    //{Name:SetWeather,"Param": "Sunny"}
    if (RigelAPIManager != nullptr)
    {
        RigelAPIManager->Invoke(Data);
    }
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
