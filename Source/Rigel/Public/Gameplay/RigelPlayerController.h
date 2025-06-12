#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PixelStreamingInputComponent.h"
#include "RigelPlayerController.generated.h"

class URigelAPIManager;

UCLASS()
class RIGEL_API ARigelPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
    ARigelPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    //接收Web端发送的消息
    UFUNCTION()
    void OnMessageByWeb(const FString& Data);

    //向web端发送消息
    void SendMessageToWeb(const FString& Data);

protected:
    virtual void BeginPlay() override;

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UPixelStreamingInput* PixelStreamingInput;
};
