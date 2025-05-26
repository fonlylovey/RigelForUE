

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "RigelGameInstance.generated.h"
/**
 * 
 */
UCLASS()
class RIGEL_API URigelGameInstance : public UGameInstance
{
    GENERATED_BODY()
public:

    UFUNCTION(BlueprintCallable, Category = "Rigel")
    static URigelGameInstance* RigelInstance();

    virtual void Init() override;

    virtual void Shutdown() override;

    virtual void OnStart();

private:
    static URigelGameInstance* Self;
};
