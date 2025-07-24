#include "WebAPI/RigelAPISubsystem.h"
#include "WebAPI/RigelFunctionLibrary.h"
#include "JsonLibraryHelpers.h"

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
    Register(TEXT("SetActorVisible"), &URigelFunctionLibrary::SetActorVisible);
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
    //{"Name":"SetVisible", "Data": {"ID":Sunny", "Name": "aaaa"}}
    FJsonLibraryObject jsonRootObj = FJsonLibraryObject::Parse(JsonData);
    FString strName = jsonRootObj.GetString(TEXT("Name"));
    //{"Data": {"ID":Sunny", "Name": "aaaa"}}  需要直接传递{"ID":Sunny", "Name": "aaaa"}
    FJsonLibraryObject dataObj = jsonRootObj.GetObject(TEXT("Data"));
    Invoke(strName, dataObj);
}

void URigelAPISubsystem::Invoke(const FString& FunName, const FJsonLibraryObject& DataObj)
{
    //FString strText = TEXT("UE call Function ：") + FunName;
   // GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, strText);

    //dataObj = {"Data": {"ID":Sunny", "Name": "aaaa"}}  需要直接传递{"ID":Sunny", "Name": "aaaa"}
    // 
    //后续有需要可以添加在哪个线程中执行的逻辑
    //AsyncTask(ENamedThreads::GameThread, {});
    if (FunctionMap.Contains(FunName))
    {
        Function function = FunctionMap[FunName];
        function(DataObj);
    }

    if (BlueprintMap.Contains(FunName))
    {
        FRigelAPIDelegate delegate = BlueprintMap[FunName];
        delegate.ExecuteIfBound(DataObj);
    }
}
