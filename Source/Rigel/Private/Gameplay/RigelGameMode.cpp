#include "Gameplay/RigelGameMode.h"
#include "Gameplay/RigelPlayerController.h"
#include "Gameplay/RigelPawn.h"

ARigelGameMode::ARigelGameMode()
{
    DefaultPawnClass = ARigelPawn::StaticClass();
    PlayerControllerClass = ARigelPlayerController::StaticClass();
}
