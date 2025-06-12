#pragma once

#include "CoreMinimal.h"
#include "Dom/JsonValue.h"
#include "RigelAPISubsystem.generated.h"
/*
* ����UE�˵�API�ӿ�
*/

DECLARE_DYNAMIC_DELEGATE_OneParam(FRigelAPIDelegate, const FString&, Param);

UCLASS(BlueprintType, Blueprintable)
class RIGEL_API URigelAPISubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()
public:

    static URigelAPISubsystem* Instance();

    //ע��C++����
    typedef TFunction<void(const FString& Param)> Function;
    void Register(const FString& Name, Function InFunction);

    //ע����ͼ����
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
    //���ú���
    void Invoke(const FString& JsonData);

    void Invoke(const FString& Name, const FString& JsonData);


private:
    URigelAPISubsystem();
    ~URigelAPISubsystem();

    TMap<FString, Function> FunctionMap;
    TMap<FString, FRigelAPIDelegate> BlueprintMap;
};
