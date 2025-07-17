#pragma once

#include "CoreMinimal.h"
#include "JsonLibraryValue.h"
#include "RigelAPISubsystem.generated.h"
/*
* ����UE�˵�API�ӿ�
*/

DECLARE_DYNAMIC_DELEGATE_OneParam(FRigelAPIDelegate, const FJsonLibraryObject&, Value);

UCLASS(BlueprintType, Blueprintable)
class RIGEL_API URigelAPISubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()
public:

    static URigelAPISubsystem* Instance();

    //�ڳ�ʼ��ʱע�����к���
    virtual void Initialize(FSubsystemCollectionBase& Collection);

    //ȡ����ʼ��ʱ�������ע�ắ��
    virtual void Deinitialize();

    //ע��C++����
    typedef TFunction<void(const FJsonLibraryObject& DataObj)> Function;
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

    //���ú��� ��������һ��������
    //{"Name":"SetVisible", "Data": {"ID":actor_1", "Visible": true}}
    void Invoke(const FString& JsonData);

    //web UIһ��������
    void Invoke(const FString& FunName, const FJsonLibraryObject& DataObj);


private:
    URigelAPISubsystem();
    ~URigelAPISubsystem();

    TMap<FString, Function> FunctionMap;
    TMap<FString, FRigelAPIDelegate> BlueprintMap;
};
