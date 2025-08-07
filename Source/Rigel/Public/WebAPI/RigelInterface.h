#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RigelInterface.generated.h"

UCLASS(BlueprintType, Blueprintable)
class URigelInterface : public UObject
{
	GENERATED_BODY()

public:

    UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
    void SetTimeOfDay(const FJsonLibraryObject& DataObj);

    UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
    void SetScalarMaterialParameterCollection(const FJsonLibraryObject& Value);

    UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
    void SetVectorMaterialParameterCollection(const FJsonLibraryObject& Value);

};
