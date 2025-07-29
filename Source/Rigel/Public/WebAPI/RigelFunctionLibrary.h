#pragma once

#include "CoreMinimal.h"
#include "JsonLibraryValue.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RigelFunctionLibrary.generated.h"

UCLASS()
class RIGEL_API URigelFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    static void ExecCommandline(const FJsonLibraryObject& Value);

    UFUNCTION(BlueprintCallable)
    static void FlyToViewpoint(const FJsonLibraryObject& Value);

    UFUNCTION(BlueprintCallable)
    static void SetActorVisible(const FJsonLibraryObject& Value);

};
