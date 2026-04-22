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
    Register(TEXT("Log"), &URigelFunctionLibrary::Log);
    Register(TEXT("FlyToViewpoint"), &URigelFunctionLibrary::FlyToViewpoint);
    Register(TEXT("FlyToActor"), &URigelFunctionLibrary::FlyToActor);
    Register(TEXT("ExecCommandline"), &URigelFunctionLibrary::ExecCommandline);
    Register(TEXT("SetActorVisible"), &URigelFunctionLibrary::SetActorVisible);
    Register(TEXT("SetActorRootVisible"), &URigelFunctionLibrary::SetActorRootVisible);
    Register(TEXT("PlayActorAnimation"), &URigelFunctionLibrary::PlayActorAnimation); 
    Register(TEXT("EnablePostProcessVolume"), &URigelFunctionLibrary::EnablePostProcessVolume);
    Register(TEXT("SetLayerVisible"), &URigelFunctionLibrary::SetLayerVisible);
    Register(TEXT("Add3DTiles"), &URigelFunctionLibrary::Add3DTiles);
    Register(TEXT("Remove3DTiles"), &URigelFunctionLibrary::Remove3DTiles);
    Register(TEXT("StartDrawing"), &URigelFunctionLibrary::StartDrawing);
    Register(TEXT("EndDrawing"), &URigelFunctionLibrary::EndDrawing);
    Register(TEXT("RemoveActor"), &URigelFunctionLibrary::RemoveActor);
    Register(TEXT("FlyToUE"), &URigelFunctionLibrary::FlyToUE);
    Register(TEXT("ClearRuntimeActors"), &URigelFunctionLibrary::ClearRuntimeActors);
    Register(TEXT("GetPawnTransform"), &URigelFunctionLibrary::GetPawnTransform);
    Register(TEXT("UseDefaultTerrain"), &URigelFunctionLibrary::UseDefaultTerrain);
    Register(TEXT("UpdateTerrainURL"), &URigelFunctionLibrary::UpdateTerrainURL);
    Register(TEXT("AddWMSServer"), &URigelFunctionLibrary::AddWMSServer);
    Register(TEXT("AddWMTSServer"), &URigelFunctionLibrary::AddWMTSServer);
    Register(TEXT("FlyToGeopoint"), &URigelFunctionLibrary::FlyToGeopoint);


    FString strClass = TEXT("/Rigel/Blueprints/BP_RigelInterface.BP_RigelInterface_C");
    UClass* RigelAPIClass = LoadClass<URigelInterface>(nullptr, *strClass);
    if (RigelAPIClass != nullptr)
    {
        RigelAPI = NewObject<URigelInterface>(this, RigelAPIClass, TEXT("RigelInterface"));

        FRigelAPIDelegate del;
        del.BindUFunction(RigelAPI, "SetTimeOfDay");
        Register(TEXT("SetTimeOfDay"), del);
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
    ////"{\"Name\":\"SetActorVisible\",\"Data\":{\"Name\":\"Device_GZ_SPJK_80\",\"Visible\":true}}"
    FJsonLibraryObject jsonRootObj = FJsonLibraryObject::Parse(JsonData);
    if (!jsonRootObj.IsValid()) {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("字符串反序列化失败"));
    }
    FString strName = jsonRootObj.GetString(TEXT("Name"));
    FJsonLibraryObject dataObj = jsonRootObj.GetObject(TEXT("Data"));
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, strName);
    Invoke(strName, dataObj);

}

void URigelAPISubsystem::Invoke(const FString& FunName, const FJsonLibraryObject& DataObj)
{
    //后续有需要可以添加在哪个线程中执行的逻辑
    //AsyncTask(ENamedThreads::GameThread, {});
    if (FunctionMap.Contains(FunName))
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("调用函数成功：" + FunName));
        Function function = FunctionMap[FunName];
        function(DataObj);
    }
    else if (BlueprintMap.Contains(FunName))
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("调用函数成功：" + FunName));
        FRigelAPIDelegate delegate = BlueprintMap[FunName];
        delegate.Execute(DataObj);
    }
}
