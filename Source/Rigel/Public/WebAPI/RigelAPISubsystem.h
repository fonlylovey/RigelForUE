#pragma once

#include "CoreMinimal.h"
#include "Dom/JsonValue.h"
#include "JsonObjectWrapper.h"
#include "RigelAPISubsystem.generated.h"
/*
* 管理UE端的API接口
*/

DECLARE_DYNAMIC_DELEGATE_OneParam(FRigelAPIDelegate, const FJsonObjectWrapper&, Param);

UCLASS(BlueprintType, Blueprintable)
class RIGEL_API URigelAPISubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()
public:

    static URigelAPISubsystem* Instance();

    //在初始化时注册所有函数
    virtual void Initialize(FSubsystemCollectionBase& Collection);

    //取消初始化时清除所有注册函数
    virtual void Deinitialize();

    //注册C++函数
    typedef TFunction<void(const FJsonObjectWrapper& ParamJsonObj)> Function;
    void Register(const FString& Name, Function InFunction);

    //注册蓝图函数
    UFUNCTION(BlueprintCallable, Category = "Rigel")
    void Register(const FString& FunctionName, FRigelAPIDelegate InDelegate);
     /*
    UFUNCTION(BlueprintCallable, Category = "Rigel")
    void RegisterBool(const FString& FunctionName, bool value);
   
    UFUNCTION(BlueprintCallable, Category = "Rigel")
    void RegisterInteger(const FString& FunctionName, int32 value);

    UFUNCTION(BlueprintCallable, Category = "Rigel")
    void RegisterFloat(const FString& FunctionName, double value);

    UFUNCTION(BlueprintCallable, Category = "Rigel")
    void RegisterString(const FString& FunctionName, const FString& value);
    */
    //调用函数
    void Invoke(const FString& JsonData);

    void Invoke(const FString& FunName, const FJsonObjectWrapper& ParamJsonObj);


private:
    URigelAPISubsystem();
    ~URigelAPISubsystem();

    TMap<FString, Function> FunctionMap;
    TMap<FString, FRigelAPIDelegate> BlueprintMap;
};
