#include "RigelGameInstance.h"


URigelGameInstance* URigelGameInstance::Self = nullptr;


URigelGameInstance* URigelGameInstance::RigelInstance()
{
    return Self;
}

void URigelGameInstance::Init()
{
    Super::Init();
    Self = this;
}

void URigelGameInstance::Shutdown()
{
    Self = nullptr;
    Super::Shutdown();
}

void URigelGameInstance::OnStart()
{

}

