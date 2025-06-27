#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RigelFunctionLibrary.generated.h"

UCLASS()
class RIGEL_API URigelFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    static void FlyToViewpoint(const FJsonObjectWrapper& ValueObj);

    UFUNCTION(BlueprintCallable)
    static void SeActorVisible(const FJsonObjectWrapper& ValueObj);

    UFUNCTION(BlueprintCallable)
    static void ExecCommandline(const FJsonObjectWrapper& ValueObj);

    UFUNCTION(BlueprintCallable)
    static void ExecCommand(const FString& ValueObj);
};
