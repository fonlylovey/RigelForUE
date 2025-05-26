#pragma once

#include "CoreMinimal.h"
#include "RigelAPIManager.generated.h"
/*
* 管理UE端的API接口
*/
DECLARE_DYNAMIC_DELEGATE_OneParam(FRigelAPIDelegate, const FString&, Data);

UCLASS(BlueprintType, Blueprintable)
class RIGEL_API URigelAPIManager : public UObject
{
    GENERATED_BODY()
public:

    //static URigelAPI* Instance();
    //static void Destory();

    //注册C++函数
    typedef TFunction<void(const FString& Params)> Function;
    void Register(const FString& Name, Function InFunction);

    //注册蓝图函数
    UFUNCTION(BlueprintCallable, Category = "Rigel")
    void Register(const FString& Name, FRigelAPIDelegate InFunction);

    //调用函数
    void Invoke(const FString& JsonData);

    //调用函数
    void Invoke(const FString& Name, const FString& Params);

private:
    URigelAPIManager();
    ~URigelAPIManager();

    static URigelAPIManager* pSelf;

    TMap<FString, Function> FunctionMap;
    TMap<FString, FRigelAPIDelegate> BlueprintMap;
};
