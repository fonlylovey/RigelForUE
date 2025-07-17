#include "WebAPI/RigelFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Gameplay/RigelPawn.h"
#include "Actors/ViewpointManager.h"
#include "JsonLibraryValue.h"
#include "JsonLibraryObject.h"
#include "Components/ViewpointComponent.h"

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

void URigelFunctionLibrary::SeActorVisible(const FJsonLibraryObject& Value)
{
    bool isShow = Value.GetBoolean(TEXT("Visible"));
    
}

void URigelFunctionLibrary::ExecCommandline(const FJsonLibraryObject& Value)
{
    FString Command = Value.GetString(TEXT("Command"));
    if (GEngine != nullptr)
    {
        GEngine->Exec(GWorld, *Command);
    }
}

void URigelFunctionLibrary::ExecCommand(const FString& Value)
{
    if (GEngine != nullptr)
    {
        GEngine->Exec(GWorld, *Value);
    }
}
