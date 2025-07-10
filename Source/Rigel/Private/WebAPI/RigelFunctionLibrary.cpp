#include "WebAPI/RigelFunctionLibrary.h"
#include "JsonObjectWrapper.h"
#include "Kismet/GameplayStatics.h"
#include "Gameplay/RigelPawn.h"
#include "Actors/ViewpointManager.h"

void URigelFunctionLibrary::FlyToViewpoint(const FJsonObjectWrapper& ValueObj)
{
    float time = ValueObj.JsonObject->GetNumberField(TEXT("Time"));
    FString viewpointID = ValueObj.JsonObject->GetStringField(TEXT("ViewPointID"));
    ARigelPawn* rigelPawn = Cast<ARigelPawn>(UGameplayStatics::GetPlayerPawn(GWorld, 0));
    FViewpoint viewpoint = AViewpointManager::GetViewpointManager()->GetViewpoint(viewpointID);
    if (rigelPawn != nullptr)
    {
        rigelPawn->FlyToViewpoint(time, viewpoint);
    }
}

void URigelFunctionLibrary::SeActorVisible(const FJsonObjectWrapper& ValueObj)
{
    bool isShow = ValueObj.JsonObject->GetBoolField(TEXT("Visible"));
    
}

void URigelFunctionLibrary::ExecCommandline(const FJsonObjectWrapper& ValueObj)
{
    FString Command = ValueObj.JsonObject->GetStringField(TEXT("Command"));
    if (GEngine != nullptr)
    {
        GEngine->Exec(GWorld, *Command);
    }
}

void URigelFunctionLibrary::ExecCommand(const FString& ValueObj)
{
    if (GEngine != nullptr)
    {
        GEngine->Exec(GWorld, *ValueObj);
    }
}
