#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "JsonLibraryValue.h"
#include "RigelPOIWidget.generated.h"

DECLARE_DYNAMIC_DELEGATE(FRigelUMGClickedDelegate);

UCLASS(BlueprintType)
class RIGEL_API URigelPOIWidget : public UUserWidget
{
public:
    GENERATED_BODY()

public:

    URigelPOIWidget(const FObjectInitializer& ObjectInitializer);

    
    UFUNCTION(BlueprintCallable)
    void OnClickedUMG();

protected:

    virtual void NativeConstruct() override;

    

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EarthBuilder")
    FString ListenerJson = TEXT("");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EarthBuilder")
    FRigelUMGClickedDelegate OnUMGClicked;
private:
    
};
