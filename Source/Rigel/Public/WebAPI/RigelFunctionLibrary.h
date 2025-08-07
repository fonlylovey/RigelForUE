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
    static void Log(const FJsonLibraryObject& Value);

    UFUNCTION(BlueprintCallable)
    static void ExecCommandline(const FJsonLibraryObject& Value);

    UFUNCTION(BlueprintCallable)
    static void FlyToViewpoint(const FJsonLibraryObject& Value);

    UFUNCTION(BlueprintCallable)
    static void FlyToActor(const FJsonLibraryObject& Value);

    UFUNCTION(BlueprintCallable)
    static void SetActorVisible(const FJsonLibraryObject& Value);

    UFUNCTION(BlueprintCallable)
    static void PlayActorAnimation(const FJsonLibraryObject& Value);

    UFUNCTION(BlueprintCallable)
    static void EnablePostProcessVolume(const FJsonLibraryObject& Value);

    UFUNCTION(BlueprintCallable)
    static void AddSplineMeshActor(const FJsonLibraryObject& Value);

    UFUNCTION(BlueprintCallable)
    static void RemoveSplineMeshActor(const FJsonLibraryObject& Value);

    UFUNCTION(BlueprintCallable)
    static void SetLayerVisible(const FJsonLibraryObject& Value);

    UFUNCTION(BlueprintCallable)
    static void Add3DTiles(const FJsonLibraryObject& Value);
    
    UFUNCTION(BlueprintCallable)
    static void Remove3DTiles(const FJsonLibraryObject& Value);


};
