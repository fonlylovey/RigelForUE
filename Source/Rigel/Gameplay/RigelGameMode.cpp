#include "RigelGameMode.h"
#include "RigelPlayerController.h"
#include "RigelPawn.h"

ARigelGameMode::ARigelGameMode()
{
    DefaultPawnClass = ARigelPawn::StaticClass();
    PlayerControllerClass = ARigelPlayerController::StaticClass();
}
