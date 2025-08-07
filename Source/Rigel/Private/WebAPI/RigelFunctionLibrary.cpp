#include "WebAPI/RigelFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Gameplay/RigelPawn.h"
#include "Actors/ViewpointManager.h"
#include "JsonLibraryValue.h"
#include "JsonLibraryObject.h"
#include "Components/ViewpointComponent.h"
#include "WebAPI/RigelLevelEditor.h"
#include "Actors/AnimationActor.h"
#include "Engine/PostProcessVolume.h"
#include "LevelInstance/LevelInstanceActor.h"

void URigelFunctionLibrary::Log(const FJsonLibraryObject& Value)
{
    FString strLog = Value.GetString(TEXT("Log"));
    if (GEngine != nullptr)
    {
        GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Green, strLog);
    }
}

void URigelFunctionLibrary::ExecCommandline(const FJsonLibraryObject& Value)
{
    FString Command = Value.GetString(TEXT("Command"));
    if (GEngine != nullptr)
    {
        GEngine->Exec(GWorld, *Command);
    }
}

void URigelFunctionLibrary::FlyToViewpoint(const FJsonLibraryObject& Value)
{
    float time = Value.GetFloat(TEXT("Time"));
    FString viewpointID = Value.GetString(TEXT("ViewPointID"));
    ARigelPawn* rigelPawn = Cast<ARigelPawn>(UGameplayStatics::GetPlayerPawn(GWorld, 0));
    FViewpoint viewpoint = AViewpointManager::GetViewpointManager()->GetViewpoint(viewpointID);
    if (rigelPawn != nullptr)
    {
        rigelPawn->FlyToViewpoint(time, viewpoint);
    }
}

void URigelFunctionLibrary::FlyToActor(const FJsonLibraryObject& Value)
{
    float time = Value.GetFloat(TEXT("Time"));
    FString actorName = Value.GetString(TEXT("Name"));
    AActor* actor = ARigelLevelEditor::RigelLevel()->FindActor(actorName);
    ARigelPawn* rigelPawn = Cast<ARigelPawn>(UGameplayStatics::GetPlayerPawn(GWorld, 0));
    if (rigelPawn != nullptr && actor != nullptr)
    {
        rigelPawn->FlyToActor(time, actor);
    }
}

void URigelFunctionLibrary::SetActorVisible(const FJsonLibraryObject& Value)
{
    FString strName = Value.GetString(TEXT("Name"));
    bool isShow = Value.GetBoolean(TEXT("Visible"));
    if (!strName.IsEmpty())
    {
        AActor* actor = ARigelLevelEditor::RigelLevel()->FindActor(strName);
        if (actor != nullptr)
        {   
            
            actor->SetActorHiddenInGame(!isShow);
            
            ALevelInstance* levelInstance = Cast<ALevelInstance>(actor);
            if (levelInstance)
            {
                levelInstance->GetLevelInstanceSubsystem()->ForEachActorInLevelInstance(levelInstance, [&isShow](AActor* SubActor)
                    {
                        SubActor->SetActorHiddenInGame(!isShow);
                        return true;
                    });
            }
        }
    }
}

void URigelFunctionLibrary::PlayActorAnimation(const FJsonLibraryObject& Value)
{
    FString strName = Value.GetString(TEXT("Name"));
    bool isPlaying = Value.GetBoolean(TEXT("Playing"));
    float duration = Value.GetFloat(TEXT("Duration"));
    if (!strName.IsEmpty())
    {
        AActor* actor = ARigelLevelEditor::RigelLevel()->FindActor(strName);
        if (actor != nullptr)
        {
            AAnimationActor* animationActor = Cast<AAnimationActor>(actor);
            if (animationActor != nullptr)
            {
                animationActor->PlayAnimation(isPlaying, duration);
            }
        }
    }
}

void URigelFunctionLibrary::EnablePostProcessVolume(const FJsonLibraryObject& Value)
{
    FString strName = Value.GetString(TEXT("Name"));
    bool isEnable = Value.GetBoolean(TEXT("Enable"));
    if (!strName.IsEmpty())
    {
        AActor* actor = ARigelLevelEditor::RigelLevel()->FindActor(strName);
        if (actor != nullptr)
        {
            APostProcessVolume* postVolume = Cast<APostProcessVolume>(actor);
            if (postVolume != nullptr)
            {
                postVolume->bEnabled = isEnable;
            }
        }
    }
}

void URigelFunctionLibrary::AddSplineMeshActor(const FJsonLibraryObject& Value)
{
   
    FString layerID = Value.GetString(TEXT("LayerID"));
    FString Url = Value.GetString(TEXT("URL"));

}

void URigelFunctionLibrary::RemoveSplineMeshActor(const FJsonLibraryObject& Value)
{
    FString layerID = Value.GetString(TEXT("LayerID"));
    FString Url = Value.GetString(TEXT("URL"));
}

void URigelFunctionLibrary::SetLayerVisible(const FJsonLibraryObject& Value)
{
    FString layerID = Value.GetString(TEXT("LayerID"));
    bool isShow = Value.GetBoolean(TEXT("Visible"));
    ARigelLevelEditor::RigelLevel()->SetLayerVisibility(layerID, isShow);
}

void URigelFunctionLibrary::Add3DTiles(const FJsonLibraryObject& Value)
{
    FString layerID = Value.GetString(TEXT("LayerID"));
    FString Url = Value.GetString(TEXT("URL"));
    ARigelLevelEditor::RigelLevel()->Add3DTiles(layerID, Url);
}

void URigelFunctionLibrary::Remove3DTiles(const FJsonLibraryObject& Value)
{
    FString layerID = Value.GetString(TEXT("LayerID"));
    ARigelLevelEditor::RigelLevel()->RemoveLayer(layerID);
}
