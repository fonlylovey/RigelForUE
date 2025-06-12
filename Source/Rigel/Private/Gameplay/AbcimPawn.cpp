// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/AbcimPawn.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Camera/CameraComponent.h"
#include "Camera/CameraActor.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"


FMinimalViewInfo GetMinimalViewInfo(const UCameraComponent* CameraComponent)
{
	FMinimalViewInfo MinimalViewInfo = FMinimalViewInfo();
	MinimalViewInfo.Location = CameraComponent->GetComponentLocation();
	MinimalViewInfo.Rotation = CameraComponent->GetComponentRotation();
	MinimalViewInfo.FOV = CameraComponent->FieldOfView;
	MinimalViewInfo.OrthoWidth = CameraComponent->OrthoWidth;
	MinimalViewInfo.OrthoNearClipPlane = CameraComponent->OrthoNearClipPlane;
	MinimalViewInfo.OrthoFarClipPlane = CameraComponent->OrthoFarClipPlane;
	MinimalViewInfo.AspectRatio = CameraComponent->AspectRatio;
	MinimalViewInfo.bConstrainAspectRatio = CameraComponent->bConstrainAspectRatio;
	MinimalViewInfo.bUseFieldOfViewForLOD = CameraComponent->bUseFieldOfViewForLOD;
	MinimalViewInfo.ProjectionMode = CameraComponent->ProjectionMode;
	MinimalViewInfo.PostProcessBlendWeight = CameraComponent->PostProcessBlendWeight;
	MinimalViewInfo.PostProcessSettings = CameraComponent->PostProcessSettings;

	return MinimalViewInfo;
}

FName AAbcimPawn::MovementComponentName(TEXT("MovementComponent0"));
FName AAbcimPawn::SpringArmComponentName(TEXT("SpringArmComponent0"));
FName AAbcimPawn::CameraComponentName(TEXT("CameraComponent0"));
//FName AAbcimPawn::WidgetInteractionComponentName(TEXT("WidgetInteractionComponentName0"));

// Sets default values
AAbcimPawn::AAbcimPawn()
	: APawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetCanBeDamaged(true);

	SetRemoteRoleForBackwardsCompat(ROLE_SimulatedProxy);
	bReplicates = true;
	NetPriority = 3.0f;

	BaseEyeHeight = 0.0f;
	bCollideWhenPlacing = false;
	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	bUseControllerRotationPitch = true;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = true;
	
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(AAbcimPawn::SpringArmComponentName);
	RootComponent = SpringArmComponent;
	SpringArmComponent->bDoCollisionTest = false;
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->TargetArmLength = 5000.0f;
	// User Defined Params for SpringArmComponent
	SpringArmLengthMin = 50.0f;
	SpringArmLengthMax = 40000000.0f;
	
	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(AAbcimPawn::MovementComponentName);
	MovementComponent->UpdatedComponent = RootComponent;
	MovementComponent->Acceleration = 10000000000.0f;
	MovementComponent->Deceleration = 10000000000.0f;
	MovementComponent->MaxSpeed = SpringArmComponent->TargetArmLength * PanScale; // 控制平移速度 
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(AAbcimPawn::CameraComponentName);
	CameraComponent->SetupAttachment(SpringArmComponent);

	//WidgetInteractionComponent = CreateDefaultSubobject<UWidgetInteractionComponent>(AAbcimPawn::WidgetInteractionComponentName);
	//WidgetInteractionComponent->SetupAttachment(CameraComponent);

	// Used to Control Pan Speed
	PanScale = 5.0f;

	ZoomIntensityScale = 1.0f;

	FocusBlendTime = 0.3f;
}

// Called when the game starts or when spawned
void AAbcimPawn::BeginPlay()
{
	Super::BeginPlay();
	
	//Add Input Mapping Context
	if (const APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	TempViewTargetCamera = GetWorld()->SpawnActor<ACameraActor>();
	// TODO: 打包会报错，显示该函数不存在
	// TempViewTargetCamera->SetActorLabel("TempViewTargetCamera0");
	FMinimalViewInfo MinimalViewInfo;
	CalcCamera(0, MinimalViewInfo);
	// 更新 TempViewTargetCamera 参数
	UpdateTempViewTargetCamera(MinimalViewInfo);
}

// Called every frame
void AAbcimPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

// Called to bind functionality to input
void AAbcimPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) 
    {
		EnhancedInputComponent->BindAction(PanAction, ETriggerEvent::Triggered, this, &AAbcimPawn::Pan);
		EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &AAbcimPawn::Zoom);
		EnhancedInputComponent->BindAction(RotateAction, ETriggerEvent::Triggered, this, &AAbcimPawn::Rotate);
		EnhancedInputComponent->BindAction(FocusAction, ETriggerEvent::Triggered, this, &AAbcimPawn::Focus);
        EnhancedInputComponent->BindAction(LeftMouseButtonUpAction, ETriggerEvent::Triggered, this, &AAbcimPawn::OnLeftMouseButtonUp);
        EnhancedInputComponent->BindAction(LeftMouseButtonDownAction, ETriggerEvent::Triggered, this, &AAbcimPawn::OnLeftMouseButtonDown);
	}
}

void AAbcimPawn::SetViewTargetWithBlend(UCameraComponent* NewViewTargetComponent, float BlendTime,
	EViewTargetBlendFunction BlendFunc, float BlendExp, bool bLockOutgoing)
{
	if (!NewViewTargetComponent) return;
	
	const FVector Start = NewViewTargetComponent->GetComponentLocation();
	const FVector Direction = FRotationMatrix(NewViewTargetComponent->GetComponentRotation()).GetUnitAxis(EAxis::X);

	FMinimalViewInfo PawnOriginViewInfo;
	this->CalcCamera(0, PawnOriginViewInfo);
	
	SetLineTraceViewTargetWithBlend(Start, Direction, HALF_WORLD_MAX, PawnOriginViewInfo, BlendTime, BlendFunc, BlendExp, bLockOutgoing);
}

void AAbcimPawn::FlyToLocationWithBlend(const FVector& Location, float Distance, float BlendTime, EViewTargetBlendFunction BlendFunc,
	float BlendExp, bool bLockOutgoing)
{
	const UWorld* World = GetWorld();
	if (!World) return;
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(World, 0);
	if (!PlayerController || !PlayerController->PlayerCameraManager) return;
	
	// 记录当前 Pawn 的 ViewInfo 并附给临时相机
	const FMinimalViewInfo PawnOriginViewInfo = PlayerController->PlayerCameraManager->GetCameraCacheView();
	UpdateTempViewTargetCamera(PawnOriginViewInfo);
	// 切换视角至临时相机位置
	PlayerController->SetViewTarget(TempViewTargetCamera);

	// 移动 Pawn 到 Location
	this->SetActorLocation(Location);
	SpringArmComponent->TargetArmLength = Distance;

	// 平滑过渡至 Pawn 的新位置，最终调取的还是 CameraManager 的函数
	PlayerController->SetViewTargetWithBlend(this, BlendTime, BlendFunc, BlendExp, bLockOutgoing);
}

void AAbcimPawn::SetViewTarget(const FAbcimCameraPointOfViewInfo& CameraPovInfo)
{
	const UWorld* World = GetWorld();
	if (!World) return;
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(World, 0);
	if (!PlayerController) return;
	
	if (IsValid(PlayerController->PlayerCameraManager))
	{
		PlayerController->PlayerCameraManager->ViewPitchMin = CameraPovInfo.MinViewPitch;
		PlayerController->PlayerCameraManager->ViewPitchMax = CameraPovInfo.MaxViewPitch;
	}

	SpringArmLengthMin = CameraPovInfo.MinArmLength;
	SpringArmLengthMax = CameraPovInfo.MaxArmLength;

	const FMinimalViewInfo& MinimalViewInfo = CameraPovInfo.ViewInfo;
	const FVector Start = MinimalViewInfo.Location;
	const FVector Direction = FRotationMatrix(MinimalViewInfo.Rotation).GetUnitAxis(EAxis::X);
	
	SetLineTraceViewTargetWithBlend(Start, Direction, HALF_WORLD_MAX, MinimalViewInfo, CameraPovInfo.BlendTime);
}

void AAbcimPawn::SetLineTraceViewTargetWithBlend(const FVector& Start, const FVector& Direction, double MaxHitDistance, const FMinimalViewInfo& NewViewInfo, float BlendTime,
                                                 EViewTargetBlendFunction BlendFunc, float BlendExp, bool bLockOutgoing)
{
	const UWorld* World = GetWorld();
	if (!World) return;
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(World, 0);
	if (!PlayerController || !PlayerController->PlayerCameraManager) return;
	
	// 记录当前 Pawn 的 ViewInfo 并附给临时相机
	const FMinimalViewInfo PawnOriginViewInfo = PlayerController->PlayerCameraManager->GetCameraCacheView();
	UpdateTempViewTargetCamera(PawnOriginViewInfo);
	// 切换视角至临时相机位置
	PlayerController->SetViewTarget(TempViewTargetCamera);

	const FRotator Rotation = FRotationMatrix::MakeFromX(Direction).Rotator();
	double NewSpringArmLength = GetSpringArmLengthByLineTrace(Start, Rotation, MaxHitDistance, SpringArmLengthMax);
	// 如果计算失败，则不改变 TargetArmLength 的值
	if (NewSpringArmLength < 0) NewSpringArmLength = SpringArmComponent->TargetArmLength;

	// 当视角发生突变导致 SpringArmLength 范围不够时，更新 SpringArmLength 的范围
	SpringArmLengthMin = NewSpringArmLength < SpringArmLengthMin ? NewSpringArmLength : SpringArmLengthMin;
	SpringArmLengthMax = NewSpringArmLength > SpringArmLengthMax ? NewSpringArmLength : SpringArmLengthMax;
	
	SetPawnPositionByCameraAndSpringArmLength(Start, Rotation, NewSpringArmLength);
	// 更新相机参数
	UpdateCameraByViewInfo(CameraComponent, NewViewInfo);
	
	// 平滑过渡至 Pawn 的新位置，最终调取的还是 CameraManager 的函数
	PlayerController->SetViewTargetWithBlend(this, BlendTime, BlendFunc, BlendExp, bLockOutgoing);
}

void AAbcimPawn::SetPawnPositionByCameraAndSpringArmLength(const FVector& CameraLocation, const FRotator& CameraRotation, const float TargetArmLength)
{
	UE_LOG(LogTemp, Verbose, TEXT("CurrentSpringArmLength: %f, NewSpringArmLength: %f"), SpringArmComponent->TargetArmLength, TargetArmLength);
	
	// 此处不应当对 TargetArmLength 进行截取修正，在执行该函数之前应当保证 TargetArmLength 的范围
	if (TargetArmLength < SpringArmLengthMin || TargetArmLength > SpringArmLengthMax) return;
	
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		const FVector ForwardDirection = FRotationMatrix(CameraRotation).GetUnitAxis(EAxis::X);
		this->SetActorLocation(CameraLocation + ForwardDirection * TargetArmLength);
		SpringArmComponent->TargetArmLength = TargetArmLength;
		// 因为 Pawn 开启了 bUseControllerRotation[Pitch|Heading|Yaw]，此处修改 PlayerController 的 Rotation
		PlayerController->SetControlRotation(CameraRotation);
	}
}

UPawnMovementComponent* AAbcimPawn::GetMovementComponent() const
{
	return MovementComponent;
}

void AAbcimPawn::SyncPawnParameter(AAbcimPawn* SourcePawn)
{
	SetActorTransform(SourcePawn->GetTransform());
	PanScale = SourcePawn->PanScale;
	SpringArmLengthMin = SourcePawn->SpringArmLengthMin;
	SpringArmLengthMax = SourcePawn->SpringArmLengthMax;
	SpringArmComponent->TargetArmLength = SourcePawn->SpringArmComponent->TargetArmLength;
}

void AAbcimPawn::Pan(const FInputActionValue& Value)
{
	const FVector CameraLocation = CameraComponent->GetComponentLocation();
	const FRotator CameraRotation = CameraComponent->GetComponentRotation();
	// 控制 SpringArmLength 的变化值，以防止发生闪烁
	const double NewSpringArmLength = GetSpringArmLengthByLineTrace(
		CameraLocation, CameraRotation, HALF_WORLD_MAX, HALF_WORLD_MAX);
	if (NewSpringArmLength > 0)
	{
		// 当地形发生突变导致 SpringArmLength 范围不够时，更新 SpringArmLength 的范围
		SpringArmLengthMin = NewSpringArmLength < SpringArmLengthMin ? NewSpringArmLength : SpringArmLengthMin;
		SpringArmLengthMax = NewSpringArmLength > SpringArmLengthMax ? NewSpringArmLength : SpringArmLengthMax;

		SetPawnPositionByCameraAndSpringArmLength(CameraLocation, CameraRotation, NewSpringArmLength);
	}
	
	const FVector2D PanVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// 修改平移速度
		UpdateMovementComponentMaxSpeed();
		
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YanRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YanRotation).GetUnitAxis(EAxis::X);

		const FVector RightDirection = FRotationMatrix(YanRotation).GetUnitAxis(EAxis::Y);
		
		AddMovementInput(ForwardDirection, -PanVector.Y);
		AddMovementInput(RightDirection, -PanVector.X);
		
		UE_LOG(LogTemp, Verbose, TEXT("Pan: %f, %f"), PanVector.Y, PanVector.X);
	}
}

void AAbcimPawn::Zoom(const FInputActionValue& Value)
{
	if (SpringArmLengthMax < SpringArmLengthMin)
	{
		UE_LOG(LogTemp, Error, TEXT("AAbcimPawn: SpringArmLengthMax shouldn't be smaller than SpringArmLengthMin"));
		return;
	}
	
	// 鼠标每滚动一下 ZoomValue 的值是固定的 -1.0、1.0、-2.0、2.0 等
	const double ZoomValue = Value.Get<float>();

	// 计算 NormalizedTargetArmLength
	double NormalizedTargetArmLength = 0.0;
	constexpr double MIN_DISTANCE = 0.0;
	constexpr double MAX_DISTANCE = 637085600; // 取地球的半径
	NormalizedTargetArmLength = FMath::Clamp(
		(SpringArmComponent->TargetArmLength - MIN_DISTANCE) / (MAX_DISTANCE - MIN_DISTANCE), 0, 1);

	constexpr double ZOOM_SCALE_FACTOR = 50000000; // 经验值
	const double DeltaTargetArmLength = ZoomIntensityCurve->GetFloatValue(NormalizedTargetArmLength) *
		ZoomIntensityScale * ZoomValue * ZOOM_SCALE_FACTOR;
	
	const float TargetArmLength = FMath::Clamp(
		SpringArmComponent->TargetArmLength - DeltaTargetArmLength,
		SpringArmLengthMin,
		SpringArmLengthMax
		);
	SpringArmComponent->TargetArmLength = TargetArmLength;

	UE_LOG(LogTemp, Verbose, TEXT("DeltaTargetArmLength: %f, TargetArmLength: %f"), DeltaTargetArmLength, TargetArmLength);
}

void AAbcimPawn::Rotate(const FInputActionValue& Value)
{
	const FVector2D RotateVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(RotateVector.X);
		AddControllerPitchInput(-RotateVector.Y);

		UE_LOG(LogTemp, Verbose, TEXT("Rotate: %f, %f"), RotateVector.X, RotateVector.Y);
	}
}

void AAbcimPawn::Focus(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Verbose, TEXT("Focus Action"));
	const UWorld* World = GetWorld();
	if (!World) return;
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(World, 0);
	if (!PlayerController) return;
	
	FVector MouseWorldLocation;
	FVector MouseWorldDirection;
	UGameplayStatics::GetPlayerController(World, 0)->DeprojectMousePositionToWorld(MouseWorldLocation, MouseWorldDirection);

	const FVector Start = CameraComponent->GetComponentLocation();
	FMinimalViewInfo PawnOriginViewInfo;
	this->CalcCamera(0, PawnOriginViewInfo);
	SetLineTraceViewTargetWithBlend(Start, MouseWorldDirection, HALF_WORLD_MAX, PawnOriginViewInfo, FocusBlendTime);
}

void AAbcimPawn::OnLeftMouseButtonUp(const FInputActionValue& Value)
{

}

void AAbcimPawn::OnLeftMouseButtonDown(const FInputActionValue& Value)
{

}

double AAbcimPawn::GetSpringArmLengthByLineTrace(const FVector& Start, const FRotator& Rotation,
                                                 double MaxHitDistance, double MaxDeltaSpringArmLength) const
{
	const UWorld* World = GetWorld();
	if (!World) return -1;
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(World, 0);
	if (!PlayerController) return -1;
	
	const FVector ForwardDirection = FRotationMatrix(Rotation).GetUnitAxis(EAxis::X);
	const FVector End = Start + ForwardDirection * MaxHitDistance;

	FHitResult OutHit;
	bool const bHit = World->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility);
	if (!bHit) return -1;

	double OutTargetArmLength = OutHit.Distance;
	// 防止 SpringArmLength 改变过大
	if (FMath::Abs(SpringArmComponent->TargetArmLength - OutTargetArmLength) > MaxDeltaSpringArmLength) return -1;
	// DrawDebugLine(World, Start, OutHit.Location, FColor::Blue, false, 10);

	return OutTargetArmLength;
}

void AAbcimPawn::UpdateMovementComponentMaxSpeed() const
{
	// 向下发射射线，求相机距离可视地面的高度
	const UWorld* World = GetWorld();
	FHitResult OutHit;
	const FVector Start = CameraComponent->GetComponentLocation();
	const FVector End = FVector(Start.X, Start.Y, Start.Z-99999999);
	bool const bHit = World ? World->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility) : false;
	if (bHit)
	{
		const double SpringArmHeight = FMath::Abs(CameraComponent->GetComponentLocation().Z - RootComponent->GetComponentLocation().Z);
		double NewMaxSpeedBase = FMath::Min(SpringArmHeight, OutHit.Distance);
		// 设置最小奇数
		NewMaxSpeedBase = FMath::Max(NewMaxSpeedBase, 1000);
		// 修改平移速度
		MovementComponent->MaxSpeed = NewMaxSpeedBase * PanScale;
		UE_LOG(LogTemp, Verbose, TEXT("Movement Max Speed: %f"), MovementComponent->MaxSpeed);
		// DrawDebugPoint(World, OutHit.Location, 20, FColor::Red, false, 1);
	}
}

void AAbcimPawn::UpdateCameraByViewInfo(TObjectPtr<UCameraComponent> CameraComponent,
	const FMinimalViewInfo& MinimalViewInfo)
{
	if (!IsValid(CameraComponent)) return;
	CameraComponent->FieldOfView = MinimalViewInfo.FOV;
	CameraComponent->OrthoWidth = MinimalViewInfo.OrthoWidth;
	CameraComponent->OrthoNearClipPlane = MinimalViewInfo.OrthoNearClipPlane;
	CameraComponent->OrthoFarClipPlane = MinimalViewInfo.OrthoFarClipPlane;
	CameraComponent->AspectRatio = MinimalViewInfo.AspectRatio;
	CameraComponent->bConstrainAspectRatio = MinimalViewInfo.bConstrainAspectRatio;
	CameraComponent->bUseFieldOfViewForLOD = MinimalViewInfo.bUseFieldOfViewForLOD;
	CameraComponent->ProjectionMode = MinimalViewInfo.ProjectionMode;
	CameraComponent->PostProcessBlendWeight = MinimalViewInfo.PostProcessBlendWeight;
	CameraComponent->PostProcessSettings = MinimalViewInfo.PostProcessSettings;
}

void AAbcimPawn::UpdateTempViewTargetCamera(const FMinimalViewInfo& MinimalViewInfo)
{
	if (!IsValid(TempViewTargetCamera)) return;
	TempViewTargetCamera->SetActorLocation(MinimalViewInfo.Location);
	TempViewTargetCamera->SetActorRotation(MinimalViewInfo.Rotation);
	FMinimalViewInfo PawnViewInfo;
	CalcCamera(0, PawnViewInfo);
	UpdateCameraByViewInfo(TempViewTargetCamera->GetCameraComponent(), PawnViewInfo);
}
