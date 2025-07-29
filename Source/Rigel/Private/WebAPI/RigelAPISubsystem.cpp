#include "WebAPI/RigelAPISubsystem.h"
#include "WebAPI/RigelFunctionLibrary.h"
#include "JsonLibraryValue.h"
#include "JsonLibraryObject.h"

URigelAPISubsystem::URigelAPISubsystem()
{
    
}

URigelAPISubsystem::~URigelAPISubsystem()
{
}

URigelAPISubsystem* URigelAPISubsystem::RigelAPISubsystem()
{
    URigelAPISubsystem* apiSubsystem = GWorld->GetGameInstance()->GetSubsystem<URigelAPISubsystem>();
    return apiSubsystem;
}

void URigelAPISubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Register(TEXT("FlyToViewpoint"), &URigelFunctionLibrary::FlyToViewpoint);
    Register(TEXT("ExecCommandline"), &URigelFunctionLibrary::ExecCommandline);
    Register(TEXT("SetActorVisible"), &URigelFunctionLibrary::SetActorVisible);

    FString strClass = TEXT("/Rigel/Blueprints/BP_RigelInterface.BP_RigelInterface_C");
    UClass* RigelAPIClass = LoadClass<URigelInterface>(nullptr, *strClass);
    if (RigelAPIClass != nullptr)
    {
        RigelAPI = NewObject<URigelInterface>(this, RigelAPIClass, TEXT("RigelInterface"));

        FRigelAPIDelegate del;
        del.BindUFunction(RigelAPI, "SetTimeOfDay");
        Register(TEXT("SetTimeOfDay"), del);

        del.BindUFunction(RigelAPI, "SetScalarMaterialParameterCollection");
        Register(TEXT("SetScalarMaterialParameterCollection"), del);

        del.BindUFunction(RigelAPI, "SetVectorMaterialParameterCollection");
        Register(TEXT("SetVectorMaterialParameterCollection"), del);
    }
    
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
    //后续有需要可以添加在哪个线程中执行的逻辑
    //AsyncTask(ENamedThreads::GameThread, {});
    if (FunctionMap.Contains(FunName))
    {
        Function function = FunctionMap[FunName];
        function(DataObj);
    }
    else if (BlueprintMap.Contains(FunName))
    {
        FRigelAPIDelegate delegate = BlueprintMap[FunName];
        delegate.ExecuteIfBound(DataObj);
    }
}
