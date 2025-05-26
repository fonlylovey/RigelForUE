#include "RigelAPIManager.h"

URigelAPIManager* URigelAPIManager::pSelf = nullptr;

URigelAPIManager::URigelAPIManager()
{
    FSimpleDelegate del;
    //del.BindUFunction
}

URigelAPIManager::~URigelAPIManager()
{
}

void URigelAPIManager::Register(const FString& Name, Function InFunction)
{
    FunctionMap.Add(Name, InFunction);
}

void URigelAPIManager::Register(const FString& Name, FRigelAPIDelegate InFunction)
{
    BlueprintMap.Add(Name, InFunction);
}

void URigelAPIManager::Invoke(const FString& Name, const FString& Params)
{
    //后续有需要可以添加在哪个线程中执行的逻辑
    //AsyncTask(ENamedThreads::GameThread, {});
    if (FunctionMap.Contains(Name))
    {
        Function function = FunctionMap[Name];
        function(Params);
    }

    if (BlueprintMap.Contains(Name))
    {
        FRigelAPIDelegate delegate = BlueprintMap[Name];
        delegate.ExecuteIfBound(Params);
    }
}

void URigelAPIManager::Invoke(const FString& JsonData)
{

}
