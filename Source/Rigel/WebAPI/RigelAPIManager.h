#pragma once

#include "CoreMinimal.h"
#include "RigelAPIManager.generated.h"
/*
* ����UE�˵�API�ӿ�
*/
DECLARE_DYNAMIC_DELEGATE_OneParam(FRigelAPIDelegate, const FString&, Data);

UCLASS(BlueprintType, Blueprintable)
class RIGEL_API URigelAPIManager : public UObject
{
    GENERATED_BODY()
public:

    //static URigelAPI* Instance();
    //static void Destory();

    //ע��C++����
    typedef TFunction<void(const FString& Params)> Function;
    void Register(const FString& Name, Function InFunction);

    //ע����ͼ����
    UFUNCTION(BlueprintCallable, Category = "Rigel")
    void Register(const FString& Name, FRigelAPIDelegate InFunction);

    //���ú���
    void Invoke(const FString& JsonData);

    //���ú���
    void Invoke(const FString& Name, const FString& Params);

private:
    URigelAPIManager();
    ~URigelAPIManager();

    static URigelAPIManager* pSelf;

    TMap<FString, Function> FunctionMap;
    TMap<FString, FRigelAPIDelegate> BlueprintMap;
};
