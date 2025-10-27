#pragma once

#include "CoreMinimal.h"
#include "JsonLibraryValue.h"
#include "RigelInterface.h"
#include "PixelStreamingInputComponent.h"
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
    
    UFUNCTION(BlueprintCallable, Category = "Rigel")
    static URigelAPISubsystem* RigelAPISubsystem();
    
    //�ڳ�ʼ��ʱע�����к���
    virtual void Initialize(FSubsystemCollectionBase& Collection);

    //ȡ����ʼ��ʱ�������ע�ắ��
    virtual void Deinitialize();

    //ע��C++����
    typedef TFunction<void(const FJsonLibraryObject& DataObj)> Function;
    void Register(const FString& Name, Function InFunction);

    //Blueprint Register  JS API
    UFUNCTION(BlueprintCallable, Category = "Rigel")
    void Register(const FString& FunctionName, FRigelAPIDelegate InDelegate);

    //���ú��� ��������һ��������
    //{"Name":"SetVisible", "Data": {"ID":actor_1", "Visible": true}}
    void Invoke(const FString& JsonData);

    //web UIһ��������
    void Invoke(const FString& FunName, const FJsonLibraryObject& DataObj);

private:
    URigelAPISubsystem();
    ~URigelAPISubsystem();

    UPROPERTY()
    URigelInterface* RigelAPI;

    UPixelStreamingInput* PixelStreaming;

    TMap<FString, Function> FunctionMap;

    TMap<FString, FRigelAPIDelegate> BlueprintMap;
};
