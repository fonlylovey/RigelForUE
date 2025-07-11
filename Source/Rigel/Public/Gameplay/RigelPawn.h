﻿#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "Camera/CameraComponent.h"
#include "RigelPawn.generated.h"

class UFloatingPawnMovement;
class UWidgetInteractionComponent;
UCLASS()
class RIGEL_API ARigelPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ARigelPawn(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    //鼠标当前点击位置的UE坐标
    FVector PickLocation();

    AActor* PickActor();

    UPrimitiveComponent* PickComponent();

    UFUNCTION(BlueprintCallable)
    void FlyToViewpoint(float time, const FViewpoint& Viewpoint);

protected:

    void Zoom(const FInputActionValue& Value);

    //绕焦点水平旋转相机
    UFUNCTION(BlueprintCallable)
    void YawRotation(const FInputActionValue& Value);

    //绕着焦点竖直旋转相机
    UFUNCTION(BlueprintCallable)
    void PitchRotation(const FInputActionValue& Value);

    void Focus(const FInputActionValue& Value);

    void OnLeftMousePressed(const FInputActionValue& Value);

    void OnLeftMouseRelease(const FInputActionValue& Value);

    void OnRightMousePressed(const FInputActionValue& Value);

    void OnRightMouseRelease(const FInputActionValue& Value);

    void MoveForward(const FInputActionValue& Value);

    void MoveRight(const FInputActionValue& Value);

    //键盘控制移动，鼠标旋转会被移动干扰，所以单独判断是鼠标左键按下才会移动，键盘的移动单独判断，实现与鼠标一样
    void MoveForward_Key(const FInputActionValue& Value);
    void MoveRight_Key(const FInputActionValue& Value);

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rigel")
    TObjectPtr<UCameraComponent> Camera;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rigel")
    TObjectPtr<UFloatingPawnMovement> FloatingMovement;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rigel")
    TObjectPtr<UWidgetInteractionComponent> WidgetInteraction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rigel")
    class AActor* FocusActor;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Rigel")
    class UViewpointComponent* ViewpointComponent;
    //增强输入时间映射
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rigel|InhancedInput")
    class UInputMappingContext* DefaultMappingContext;

    //水平方向移动
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rigel|InhancedInput")
    class UInputAction* MoveForwardAction;

    //水平方向移动
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rigel|InhancedInput")
    class UInputAction* MoveRightAction;

    //水平方向移动
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rigel|InhancedInput")
    class UInputAction* KeyMoveForwardAction;

    //水平方向移动
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rigel|InhancedInput")
    class UInputAction* KeyMoveRightAction;

    //zoom 拉近拉远
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rigel|InhancedInput")
    class UInputAction* ZoomAction;

    //水平环绕旋转
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rigel|InhancedInput")
    class UInputAction* YawRotationAction;

    //竖直上下旋转
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rigel|InhancedInput")
    class UInputAction* PitchRotationAction;

    //鼠标左键按下
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rigel|InhancedInput")
    class UInputAction* LeftMousePressedAction;

    //鼠标左键释放
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rigel|InhancedInput")
    class UInputAction* LeftMouseReleaseAction;

    //鼠标右键按下
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rigel|InhancedInput")
    class UInputAction* RightMousePressedAction;

    //鼠标右键释放
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rigel|InhancedInput")
    class UInputAction* RightMouseReleaseAction;

private:
    double Distance = 0.0;
    bool IsMouseLeft = false;
    bool IsMouseRight = false;
    //鼠标当前点击时和场景交点的世界坐标
    FVector PickWorldLocation;

};
