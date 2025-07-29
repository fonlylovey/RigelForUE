#pragma once

#include "CoreMinimal.h"
#include "JsonLibraryValue.h"
#include "RigelInterface.h"
#include "RigelAPISubsystem.generated.h"
/*
* 管理UE端的API接口
*/

DECLARE_DYNAMIC_DELEGATE_OneParam(FRigelAPIDelegate, const FJsonLibraryObject&, Value);

UCLASS(BlueprintType, Blueprintable)
class RIGEL_API URigelAPISubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()
public:
    
    UFUNCTION(BlueprintCallable, Category = "Rigel")
    static URigelAPISubsystem* RigelAPISubsystem();
    
    //在初始化时注册所有函数
    virtual void Initialize(FSubsystemCollectionBase& Collection);

    //取消初始化时清除所有注册函数
    virtual void Deinitialize();

    //注册C++函数
    typedef TFunction<void(const FJsonLibraryObject& DataObj)> Function;
    void Register(const FString& Name, Function InFunction);

    //注册蓝图函数
    UFUNCTION(BlueprintCallable, Category = "Rigel")
    void Register(const FString& FunctionName, FRigelAPIDelegate InDelegate);

    //调用函数 像素流送一般调用这个
    //{"Name":"SetVisible", "Data": {"ID":actor_1", "Visible": true}}
    void Invoke(const FString& JsonData);

    //web UI一般调用这个
    void Invoke(const FString& FunName, const FJsonLibraryObject& DataObj);

private:
    URigelAPISubsystem();
    ~URigelAPISubsystem();

    UPROPERTY()
    URigelInterface* RigelAPI;

    TMap<FString, Function> FunctionMap;

    TMap<FString, FRigelAPIDelegate> BlueprintMap;
};
