#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VlcPlayer.generated.h"

UCLASS(BlueprintType)
class RIGEL_API UVlcPlayer : public UUserWidget
{
public:
    GENERATED_BODY()

public:

    UVlcPlayer(const FObjectInitializer& ObjectInitializer);

    UFUNCTION(BlueprintCallable)
    void OpenURL(const FString& URL);

protected:

    virtual void NativeConstruct() override;

public:
    

private:

};
