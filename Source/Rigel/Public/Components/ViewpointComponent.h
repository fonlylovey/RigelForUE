#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Math/MathFwd.h"
#include "ViewpointComponent.generated.h"

class UCameraComponent;

USTRUCT(BlueprintType)
struct FViewpoint
{
    GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Name;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UTexture2D* Thumbnail;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector Location;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FRotator Rotation;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UViewpointComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UViewpointComponent();

protected:
	// Called when the game star
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UFUNCTION(BlueprintCallable, Category = "Rigel", meta=(DisplayName="添加当前位置到视点"), CallInEditor)
    void AddViewpoint();

    void RoamingToViewpoint(float time, const FString& ViewpointID);

public:
    UPROPERTY(Category = "Rigel", EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "视点列表"))
    TMap<FString, FViewpoint> ViewpointList;

private:
    //漫游的时间
    float Duration = 0.0;
    //已经漫游的用时
    float Runtime = 0.0;
    FTransform StartTransform;
    FTransform TargetTransform;
};
