#include "WebAPI/RigelFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Gameplay/RigelPawn.h"
#include "Actors/ViewpointManager.h"
#include "JsonLibraryValue.h"
#include "JsonLibraryObject.h"
#include "Components/ViewpointComponent.h"
#include "WebAPI/RigelLevelEditor.h"

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
        }
    }
}
