#include "WebAPI/RigelAPISubsystem.h"
#include "JsonObjectWrapper.h"


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

void URigelAPISubsystem::Register(const FString& Name, Function InFunction)
{
    FunctionMap.Add(Name, InFunction);
}

void URigelAPISubsystem::Register(const FString& Name, FRigelAPIDelegate InDelegate)
{
    BlueprintMap.Add(Name, InDelegate);
}

void URigelAPISubsystem::Invoke(const FString& Name, const FString& JsonData)
{
    FString strText = TEXT("���ú�����") + Name;
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, strText);
    //{"Name":"SetVisible", Param": "Sunny"}

    //��������Ҫ����������ĸ��߳���ִ�е��߼�
    //AsyncTask(ENamedThreads::GameThread, {});
    if (FunctionMap.Contains(Name))
    {
        Function function = FunctionMap[Name];
        function(JsonData);
    }

    if (BlueprintMap.Contains(Name))
    {
        FRigelAPIDelegate delegate = BlueprintMap[Name];
        delegate.ExecuteIfBound(JsonData);
    }
}

void URigelAPISubsystem::Invoke(const FString& JsonData)
{
    FJsonObjectWrapper jsonWrapper;
    jsonWrapper.JsonObjectFromString(JsonData);
    FString strName = jsonWrapper.JsonObject->GetStringField(TEXT("Name"));
    Invoke(strName, JsonData);
}
