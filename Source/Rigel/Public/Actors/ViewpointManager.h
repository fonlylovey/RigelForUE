#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ViewpointManager.generated.h"

UCLASS()
class AViewpointManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AViewpointManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable, Category = "Rigel", meta = (DisplayName = "��ӵ�ǰλ�õ��ӵ�"), CallInEditor)
    void AddViewpoint();

    UFUNCTION(BlueprintCallable,Category = "Rigel")
    static AViewpointManager* GetViewpointManager();

    UFUNCTION(BlueprintCallable, Category = "Rigel")
    FViewpoint GetViewpoint(const FString& ViewpointID);

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Rigel")
    class UViewpointComponent* Viewpoint;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rigel")
    class UCameraComponent* Camera;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Rigel")
    class USceneCaptureComponent2D* SceneCapture;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Rigel")
    class UTextureRenderTarget2D* RenderTarget;

    UPROPERTY(Category = "Rigel", EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "�ӵ��б�"))
    TMap<FString, FViewpoint> ViewpointList;
};
