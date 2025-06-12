#include "Gameplay/RigelPlayerController.h"
#include "WebAPI/RigelAPISubsystem.h"

ARigelPlayerController::ARigelPlayerController(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    PixelStreamingInput = CreateDefaultSubobject<UPixelStreamingInput>(TEXT("PixelStreamingInput"));
    PixelStreamingInput->OnInputEvent.AddDynamic(this, &ARigelPlayerController::OnMessageByWeb);
}

void ARigelPlayerController::OnMessageByWeb(const FString& Data)
{
    //{Name:SetWeather,"Param": "Sunny"}
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, Data);
    URigelAPISubsystem* subsystem = GetWorld()->GetGameInstance()->GetSubsystem<URigelAPISubsystem>();
    subsystem->Invoke(Data);
    URigelAPISubsystem::Instance();
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
