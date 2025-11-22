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

    //����Web�˷��͵���Ϣ
    UFUNCTION(BlueprintCallable)
    void OnMessageByWeb(const FString& Data);

    //
    UFUNCTION(BlueprintCallable)
    void SendMessageToWeb(const FString& Data);

protected:
    virtual void BeginPlay() override;

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UPixelStreamingInput* PixelStreamingInput;
};
