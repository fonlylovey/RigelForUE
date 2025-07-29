#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RigelLevelEditor.generated.h"

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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Rigel")
    AActor* UDS;

	UPROPERTY(EditAnywhere, Blueprintable, BlueprintReadWrite, Category = "Rigel")
	TMap<FString, AActor*> ActorMap;
};
