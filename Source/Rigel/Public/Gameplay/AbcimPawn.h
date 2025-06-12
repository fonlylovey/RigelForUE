// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "InputActionValue.h"
#include "AbcimPawn.generated.h"

class UFloatingPawnMovement;
class USphereComponent;
class UStaticMeshComponent;
class USpringArmComponent;
class UCameraComponent;
class UCurveFloat;

USTRUCT(BlueprintType, Blueprintable)
struct FAbcimCameraPointOfViewInfo
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, Category = "Spring Arm", meta = (ToolTip = "Spring arm default length."))
    float DefaultArmLength = 1000.0f;

    UPROPERTY(EditAnywhere, Category = "Spring Arm", meta = (ClampMin = 0.0, UIMin = 0.0, ToolTip = "Min spring arm length."))
    float MinArmLength = 50.0f;

    UPROPERTY(EditAnywhere, Category = "Spring Arm", meta = (ClampMin = 0.0001, UIMin = 0.0001, ToolTip = "Max spring arm length."))
    float MaxArmLength = 40000000.0f;

    UPROPERTY(EditAnywhere, Category = "Rotation", meta = (UIMin = -89.9999, UIMax = 89.9999, ToolTip = "Min rotation pitch."))
    float MinViewPitch = -80.0f;

    UPROPERTY(EditAnywhere, Category = "Rotation", meta = (UIMin = -89.9999, UIMax = 89.9999, ToolTip = "Max rotation pitch"))
    float MaxViewPitch = 0.0f;

    UPROPERTY(EditAnywhere, Category = "Animation", meta = (ClampMin = 0.0, UIMin = 0.0, ToolTip = "Time taken to blend"))
    float BlendTime = 0.5f;

    UPROPERTY(EditAnywhere, Category = "", meta = (ClampMin = 0.0, UIMin = 0.0, ToolTip = "Camera view info."))
    FMinimalViewInfo ViewInfo;
};

UCLASS()
class RIGEL_API AAbcimPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AAbcimPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Begin Pawn overrides
	virtual UPawnMovementComponent* GetMovementComponent() const override;
	// End Pawn overrides

	/**
	 * Copy parameter from another pawn to self.
	 * @param SourcePawn 
	 */
	UFUNCTION(BlueprintCallable, Category = "Abcim")
	virtual void SyncPawnParameter(AAbcimPawn* SourcePawn);

protected:
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|InhancedInput", meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Pan Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|InhancedInput", meta = (AllowPrivateAccess = "true"))
	class UInputAction* PanAction;

	/** Zoom Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|InhancedInput", meta = (AllowPrivateAccess = "true"))
	class UInputAction* ZoomAction;

	/** Rotate Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|InhancedInput", meta = (AllowPrivateAccess = "true"))
	class UInputAction* RotateAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|InhancedInput", meta = (AllowPrivateAccess = "true"))
	class UInputAction* FocusAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|InhancedInput", meta = (AllowPrivateAccess = "true"))
    class UInputAction* LeftMouseButtonUpAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|InhancedInput", meta = (AllowPrivateAccess = "true"))
    class UInputAction* LeftMouseButtonDownAction;

	/**
	 * 用于控制平移速度
	 * MovementComponent->MaxSpeed = SpringArmComponent->TargetArmLength * PanScale 
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Pan", meta = (AllowPrivateAccess = "true"))
	float PanScale;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Zoom", meta = (AllowPrivateAccess = "true"))
	float SpringArmLengthMin;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Zoom", meta = (AllowPrivateAccess = "true"))
	float SpringArmLengthMax;

	/**
	 * 用于控制不同 SpringArmLength 时 Zoom 速率的曲线，X 轴范围为 0-1，y 轴的范围也为 0-1
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Zoom", meta = (AllowPrivateAccess = "true"))
	UCurveFloat* ZoomIntensityCurve;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Zoom", meta = (AllowPrivateAccess = "true"))
	float ZoomIntensityScale;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Focus", meta = (AllowPrivateAccess = "true"))
	float FocusBlendTime;
	
	void Pan(const FInputActionValue& Value);

	void Zoom(const FInputActionValue& Value);

	void Rotate(const FInputActionValue& Value);

	void Focus(const FInputActionValue& Value);
	
    void OnLeftMouseButtonUp(const FInputActionValue& Value);

    void OnLeftMouseButtonDown(const FInputActionValue& Value);

	UPROPERTY(Category = Pawn, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UFloatingPawnMovement> MovementComponent;

public:
	/** Name of the MovementComponent.  Use this name if you want to use a different class (with ObjectInitializer.SetDefaultSubobjectClass). */
	static FName MovementComponentName;

protected:
	UPROPERTY(Category = Pawn, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	/**
	 * @brief 通过发出射线求 SpringArmLength
	 * @param Start 射线起点位置
	 * @param Rotation 射线 Rotation
	 * @param MaxHitDistance 最大射线长度
	 * @param MaxDeltaSpringArmLength SpringArm允许改变的最大长度
	 * @return 计算成功则返回大于 0 的值，失败则返回 -1
	 */
	double GetSpringArmLengthByLineTrace(const FVector& Start, const FRotator& Rotation,
	                                                double MaxHitDistance, double MaxDeltaSpringArmLength) const;

	void UpdateMovementComponentMaxSpeed() const;

public:
	/** Name of the SpringArmComponent. */
	static FName SpringArmComponentName;

	/** Returns SpringArmComponent */
	USpringArmComponent* GetSpringArmComponent() const { return SpringArmComponent; }

protected:
	UPROPERTY(Category = Pawn, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> CameraComponent;

	/** Temp View Target Camera. Used for Focus Input Action*/
	TObjectPtr<ACameraActor> TempViewTargetCamera;

	/**
	 * 通过 MinimalViewInfo 更新相机参数，相机的位置和旋转除外
	 * @param CameraComponent 
	 * @param MinimalViewInfo 
	 */
	static void UpdateCameraByViewInfo(TObjectPtr<UCameraComponent> CameraComponent, const FMinimalViewInfo& MinimalViewInfo);
	
	/** 通过摄像机参数更新临时相机的位置 */
	void UpdateTempViewTargetCamera(const FMinimalViewInfo& MinimalViewInfo);

public:
	/** Name of the CameraComponent. */
	static FName CameraComponentName;

	/** Returns CameraComponent */
	UCameraComponent* GetCameraComponent() const { return CameraComponent; }

protected:
	//UPROPERTY(Category = Pawn, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	//TObjectPtr<UWidgetInteractionComponent> WidgetInteractionComponent;

public:
	/** Name of the WidgetInteractionComponent. */
	//static FName WidgetInteractionComponentName;

	/** Returns WidgetInteractionComponent */
	//UWidgetInteractionComponent* GetWidgetInteractionComponent() const { return WidgetInteractionComponent; }

public:
	/**
	 * Set the view target blending with variable control
	 * @param NewViewTargetComponent - new component to set as view target
	 * @param BlendTime time taken to blend
	 * @param BlendFunc Cubic, Linear etc functions for blending
	 * @param BlendExp Exponent, used by certain blend functions to control the shape of the curve. 
	 * @param bLockOutgoing If true, lock outgoing viewtarget to last frame's camera position for the remainder of the blend.
	 */
	UFUNCTION(BlueprintCallable, Category="View", meta=(Keywords = "Camera"))
	virtual void SetViewTargetWithBlend(UCameraComponent* NewViewTargetComponent, float BlendTime = 0, enum EViewTargetBlendFunction BlendFunc = VTBlend_Linear, float BlendExp = 0, bool bLockOutgoing = false);

	/**
	 * 
	 * @param Location location to fly to
	 * @param Distance distance between location and camera
	 * @param BlendTime time taken to blend
	 * @param BlendFunc Cubic, Linear etc functions for blending
	 * @param BlendExp Exponent, used by certain blend functions to control the shape of the curve. 
	 * @param bLockOutgoing If true, lock outgoing viewtarget to last frame's camera position for the remainder of the blend.
	 */
	UFUNCTION(BlueprintCallable, Category="View", meta=(Keywords = "Camera"))
	virtual void FlyToLocationWithBlend(const FVector& Location, float Distance = 1000.0, float BlendTime = 1.0, enum EViewTargetBlendFunction BlendFunc = VTBlend_Linear, float BlendExp = 0, bool bLockOutgoing = false);
	/**
	 * Set the view target by FAbcimCameraPointOfViewInfo
	 * @param CameraPovInfo - View info with a camera component.
	 */
	UFUNCTION(BlueprintCallable, Category="View", meta=(Keywords = "Camera"))
	virtual void SetViewTarget(const FAbcimCameraPointOfViewInfo& CameraPovInfo);

protected:
	/**
	 * Help function, used for set view target by line trace.
	 * @param Start View start point.
	 * @param Direction View direction. 
	 * @param MaxHitDistance Max line trace hit distance.
	 * @param NewViewInfo Camera info.
	 * @param BlendTime time taken to blend
	 * @param BlendFunc Cubic, Linear etc functions for blending
	 * @param BlendExp Exponent, used by certain blend functions to control the shape of the curve. 
	 * @param bLockOutgoing If true, lock outgoing viewtarget to last frame's camera position for the remainder of the blend.
	 */
	void SetLineTraceViewTargetWithBlend(const FVector& Start, const FVector& Direction, double MaxHitDistance, const FMinimalViewInfo& NewViewInfo, float BlendTime = 0, enum EViewTargetBlendFunction BlendFunc = VTBlend_Linear, float BlendExp = 0, bool bLockOutgoing = false);
	
	virtual void SetPawnPositionByCameraAndSpringArmLength(const FVector& CameraLocation, const FRotator& CameraRotation, const float TargetArmLength);
};
