#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RigelLevelEditor.generated.h"

struct FJsonLibraryValue;
class ASplinePathMesh;
class ACesium3DTileset;
class UCesiumRasterOverlay;

struct ServiceOption
{
    FString Layers = TEXT("");
    int MinLevel = 0;
    int MaxLevel = 18;
};

UCLASS()
class ARigelLevelEditor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARigelLevelEditor();

    virtual void OnConstruction(const FTransform& Transform) override;

	UFUNCTION(BlueprintCallable,Category = "Rigel")
	static ARigelLevelEditor* RigelLevel();

    void InitOption();

    UFUNCTION(BlueprintCallable, Category = "Rigel", meta = (Tooltip = "从Map中获取Actor"))
	AActor* GetActor(const FString& Name);

    UFUNCTION(BlueprintCallable, Category = "Rigel", meta = (Tooltip = "从运行时创建的Actor容器中获取Actor"))
    AActor* GetRuntimeActor(const FString& Name);

    UFUNCTION(BlueprintCallable, Category = "Rigel", meta = (Tooltip = "从运行时创建的Actor容器中删除Actor"))
    void RemoveRuntimeActor(const FString& Name);

    UFUNCTION(BlueprintCallable, Category = "Rigel", meta = (Tooltip = "从关卡中查找Actor"))
    AActor* FindActor(const FString& Name);

    UFUNCTION(BlueprintCallable, Category = "Rigel", meta = (Tooltip = "Add Actor To Actor Map"))
    void AddActorToActorMap(const FString& Name, AActor* Actor);

    UFUNCTION(BlueprintCallable, Category = "Rigel", meta = (Tooltip = "Add Actor To Actor Layer"))
    void AddActorToLayerMap(const FString& Name, AActor* Actor);

    void SetLayerVisibility(const FString& LayerID, bool isVisible);
    void RemoveLayer(const FString& LayerID);

    void Add3DTiles(const FString& LayerID, const FString& Url, float Height);
    void AddSplineMesh();

    void AddSelect(UPrimitiveComponent* component);
    void ClearSelect();

    UFUNCTION(BlueprintCallable, Category = "Rigel", meta = (Tooltip = "发送消息给WebUI"))
    void SendMessageToWeb(const FString& Function, const FJsonLibraryValue& Data);

    void SetWebWIdget(class UWebWidget* widget);

    UFUNCTION(BlueprintCallable, Category = "Rigel", meta = (Tooltip = "发送消息给WebUI"))
    void StartDrawGeometry(const FString& ActorName);

    UFUNCTION()
    void EndDrawGeometry();

    UFUNCTION()
    void OnDrawing(const FVector& Location);

    UFUNCTION(BlueprintCallable, Category = "Rigel", meta = (Tooltip = "移除ID为ActorID的POI"))
    void RemoveActor(const FString& ActorID);

    ASplinePathMesh* CreateSplineActor(const FString& ActorName);

    UFUNCTION(BlueprintCallable, Category = "Rigel", meta = (Tooltip = "移除所有运行时创建的所有POI"))
    void ClearRuntimeActors();

    UFUNCTION(BlueprintCallable, Category = "Rigel")
    void LoadHtmlUrl();

    void LoadInitServer();

    void UseDefaultTerrain();

    void UpdateTerrainURL(const FString& TerrainUrl);

    void AddWMSServer(const FString& LayerID, const FString& BaseUrl, const ServiceOption& option);

    void AddWMTSServer(const FString& LayerID, const FString& BaseUrl, const ServiceOption& option);

    UFUNCTION(BlueprintCallable, Category = "Rigel")
    UWebWidget* GetWebUI() { return WebWidget; }
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rigel")
    AActor* UDS;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rigel")
    ACesium3DTileset* World;

	UPROPERTY(EditAnywhere, Blueprintable, BlueprintReadWrite, Category = "Rigel")
	TMap<FString, AActor*> ActorMap;

    UPROPERTY(EditAnywhere, Blueprintable, BlueprintReadWrite, Category = "Rigel")
    TMap<FString, AActor*> LayerMap;


    UPROPERTY(EditAnywhere, Blueprintable, BlueprintReadWrite, Category = "Rigel")
    UMaterialInstance* SelectMaterial;

    UPROPERTY(EditAnywhere, Blueprintable, BlueprintReadWrite, Category = "Rigel")
    class AGeoReferencingSystem* GeoReferencing = nullptr;

    UPROPERTY(EditAnywhere, Blueprintable, BlueprintReadWrite, Category = "Rigel")
    TMap<FString, AActor*> RuntimeMap;

    UPROPERTY(EditAnywhere, Blueprintable, BlueprintReadWrite, Category = "Rigel")
    TMap<FString, UCesiumRasterOverlay*> ServerMap;

    UPROPERTY(Blueprintable, BlueprintReadOnly, Category = "Rigel")
    FString HtmlURL;

    UPROPERTY(Blueprintable, BlueprintReadOnly, Category = "Rigel")
    int StreamMode = 0;

    UPROPERTY(Blueprintable, BlueprintReadOnly, Category = "Rigel")
    FString GISServerIP = TEXT("");
private:
    TArray<UPrimitiveComponent*> SelectSet;

    class UWebWidget* WebWidget;

    UPROPERTY()
    ASplinePathMesh* DrawingSplineActor;

    FString ProjectOptionPath;
    
};
