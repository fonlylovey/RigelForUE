#include "WebAPI/RigelAPISubsystem.h"
#include "JsonObjectWrapper.h"
#include "WebAPI/RigelFunctionLibrary.h"

URigelAPISubsystem::URigelAPISubsystem()
{
    
}

URigelAPISubsystem::~URigelAPISubsystem()
{
}

URigelAPISubsystem* URigelAPISubsystem::Instance()
{
    URigelAPISubsystem* apiSubsystem = GWorld->GetGameInstance()->GetSubsystem<URigelAPISubsystem>();
    return apiSubsystem;
}

void URigelAPISubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Register(TEXT("FlyToViewpoint"), &URigelFunctionLibrary::FlyToViewpoint);
    Register(TEXT("ExecCommandline"), &URigelFunctionLibrary::ExecCommandline);
}

void URigelAPISubsystem::Deinitialize()
{
    FunctionMap.Empty(0);
    BlueprintMap.Empty(0);
}

void URigelAPISubsystem::Register(const FString& Name, Function InFunction)
{
    FunctionMap.Add(Name, InFunction);
}

void URigelAPISubsystem::Register(const FString& Name, FRigelAPIDelegate InDelegate)
{
    BlueprintMap.Add(Name, InDelegate);
}

void URigelAPISubsystem::Invoke(const FString& JsonData)
{
    //初步解析json
    //{"Name":"SetVisible", "Param": {"ID":Sunny", "Name": "aaaa"}}
    FJsonObjectWrapper jsonWrapper;
    jsonWrapper.JsonObjectFromString(JsonData);
    FString strName = jsonWrapper.JsonObject->GetStringField(TEXT("Name"));
    TSharedPtr<FJsonObject> ParamObjPtr = jsonWrapper.JsonObject->GetObjectField(TEXT("Param"));
    FJsonObjectWrapper wrapper;
    wrapper.JsonObject = ParamObjPtr;
    Invoke(strName, wrapper);
}

void URigelAPISubsystem::Invoke(const FString& FunName, const FJsonObjectWrapper& ParmObj)
{
    FString strText = TEXT("UE call Function ：") + FunName;
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, strText);
    

    //后续有需要可以添加在哪个线程中执行的逻辑
    //AsyncTask(ENamedThreads::GameThread, {});
    if (FunctionMap.Contains(FunName))
    {
        Function function = FunctionMap[FunName];
        function(ParmObj);
    }

    if (BlueprintMap.Contains(FunName))
    {
        FRigelAPIDelegate delegate = BlueprintMap[FunName];
        delegate.ExecuteIfBound(ParmObj);
    }
}
