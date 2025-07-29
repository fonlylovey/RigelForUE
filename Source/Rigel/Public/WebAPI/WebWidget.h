#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WebInterface.h"
#include "Components/CanvasPanel.h"
#include "JsonLibraryValue.h"
#include "WebInterfaceCallback.h"
#include "WebWidget.generated.h"

UCLASS(BlueprintType)
class RIGEL_API UWebWidget : public UUserWidget
{
public:
    GENERATED_BODY()

public:

    UWebWidget(const FObjectInitializer& ObjectInitializer);

    UFUNCTION(BlueprintCallable)
    void LoadHTML(const FString& URL);

    UFUNCTION(BlueprintCallable)
    void LoadFile(const FString& ContentFile);

protected:

    virtual void NativeConstruct() override;

    //virtual TSharedRef<SWidget> RebuildWidget() override;

    UFUNCTION(BlueprintCallable)
    void OnInterfaceEvent(const FName Name, FJsonLibraryValue Data, FWebInterfaceCallback Callback);

public:
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UWebInterface* WebInterface;

private:
    
};
