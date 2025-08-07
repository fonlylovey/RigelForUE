#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RigelLevelEditor.generated.h"

struct FJsonLibraryValue;

UCLASS()
class ARigelLevelEditor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARigelLevelEditor();

	UFUNCTION(BlueprintCallable,Category = "Rigel")
	static ARigelLevelEditor* RigelLevel();

    UFUNCTION(BlueprintCallable, Category = "Rigel", meta = (Tooltip = "从Map中获取Actor"))
	AActor* GetActor(const FString& Name);

    UFUNCTION(BlueprintCallable, Category = "Rigel", meta = (Tooltip = "从关卡中查找Actor"))
    AActor* FindActor(const FString& Name);

    void SetLayerVisibility(const FString& LayerID, bool isVisible);
    void RemoveLayer(const FString& LayerID);

    void Add3DTiles(const FString& LayerID, const FString&);
    void AddSplineMesh();

    void AddSelect(UPrimitiveComponent* component);
    void ClearSelect();

    UFUNCTION(BlueprintCallable, Category = "Rigel", meta = (Tooltip = "发送消息给WebUI"))
    void SendMessageToWeb(const FString& Function, const FJsonLibraryValue& Data);

    void SetWebWIdget(class UWebWidget* widget);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rigel")
    AActor* UDS;

	UPROPERTY(EditAnywhere, Blueprintable, BlueprintReadWrite, Category = "Rigel")
	TMap<FString, AActor*> ActorMap;

    UPROPERTY(EditAnywhere, Blueprintable, BlueprintReadWrite, Category = "Rigel")
    TMap<FString, AActor*> LayerMap;


    UPROPERTY(EditAnywhere, Blueprintable, BlueprintReadWrite, Category = "Rigel")
    UMaterialInstance* SelectMaterial;

private:
    TArray<UPrimitiveComponent*> SelectSet;

    class UWebWidget* WebWidget;
};
