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
    static void FlyToUE(const FJsonLibraryObject& Value);

    UFUNCTION(BlueprintCallable)
    static void FlyToGeopoint(const FJsonLibraryObject& Value);

    UFUNCTION(BlueprintCallable)
    static void FlyToActor(const FJsonLibraryObject& Value);

    UFUNCTION(BlueprintCallable)
    static void SetActorVisible(const FJsonLibraryObject& Value);

    UFUNCTION(BlueprintCallable)
    static void SetActorRootVisible(const FJsonLibraryObject& Value);

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

    UFUNCTION(BlueprintCallable)
    static void StartDrawing(const FJsonLibraryObject& Value);

    UFUNCTION(BlueprintCallable)
    static void EndDrawing(const FJsonLibraryObject& Value);

    UFUNCTION(BlueprintCallable)
    static void RemoveActor(const FJsonLibraryObject& Value);

    UFUNCTION(BlueprintCallable)
    static void ClearRuntimeActors(const FJsonLibraryObject& Value);

    UFUNCTION(BlueprintCallable)
    static void GetPawnTransform(const FJsonLibraryObject& Value);

    UFUNCTION(BlueprintCallable)
    static void UseDefaultTerrain(const FJsonLibraryObject& Value);

    UFUNCTION(BlueprintCallable)
    static void UpdateTerrainURL(const FJsonLibraryObject& Value);

    UFUNCTION(BlueprintCallable)
    static void AddWMSServer(const FJsonLibraryObject& Value);

    UFUNCTION(BlueprintCallable)
    static void AddWMTSServer(const FJsonLibraryObject& Value);

};
