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

    UFUNCTION(BlueprintCallable, Category = "Rigel", meta = (DisplayName = "添加当前位置到视点"), CallInEditor)
    void AddViewpoint();

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Rigel")
    class UViewpointComponent* Viewpoint;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rigel")
    class UCameraComponent* Camera;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Rigel")
    class USceneCaptureComponent2D* SceneCapture;
};
