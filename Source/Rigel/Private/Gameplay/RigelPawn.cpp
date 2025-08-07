#include "Gameplay/RigelPawn.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "Components/WidgetInteractionComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Components/ViewpointComponent.h"
#include "WebAPI/RigelAPISubsystem.h"
#include "WebAPI/RigelLevelEditor.h"

// Sets default values
ARigelPawn::ARigelPawn(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    RootComponent = CreateDefaultSubobject<USceneComponent>("RootComponent");
    Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
    Camera->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
    WidgetInteraction = CreateDefaultSubobject<UWidgetInteractionComponent>("WidgetInteraction");
    WidgetInteraction->SetupAttachment(RootComponent);

    FloatingMovement = CreateDefaultSubobject<UFloatingPawnMovement>("FloatingMovement");
    //FloatingMovement->UpdatedComponent = RootComponent;
    ViewpointComponent = CreateDefaultSubobject<UViewpointComponent>("Viewpoint");
}

// Called when the game starts or when spawned
void ARigelPawn::BeginPlay()
{
	Super::BeginPlay();
    const APlayerController* PlayerController = Cast<APlayerController>(Controller);
    //Add Input Mapping Context
    if (PlayerController != nullptr)
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(DefaultMappingContext, 0);
        }
    }
}

// Called every frame
void ARigelPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARigelPawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
    {
        EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &ARigelPawn::Zoom);
        EnhancedInputComponent->BindAction(YawRotationAction, ETriggerEvent::Triggered, this, &ARigelPawn::YawRotation);
        EnhancedInputComponent->BindAction(PitchRotationAction, ETriggerEvent::Triggered, this, &ARigelPawn::PitchRotation);
        EnhancedInputComponent->BindAction(LeftMousePressedAction, ETriggerEvent::Triggered, this, &ARigelPawn::OnLeftMousePressed);
        EnhancedInputComponent->BindAction(LeftMouseReleaseAction, ETriggerEvent::Triggered, this, &ARigelPawn::OnLeftMouseRelease);
        EnhancedInputComponent->BindAction(RightMousePressedAction, ETriggerEvent::Triggered, this, &ARigelPawn::OnRightMousePressed);
        EnhancedInputComponent->BindAction(RightMouseReleaseAction, ETriggerEvent::Triggered, this, &ARigelPawn::OnRightMouseRelease);
        EnhancedInputComponent->BindAction(MoveForwardAction, ETriggerEvent::Triggered, this, &ARigelPawn::MoveForward);
        EnhancedInputComponent->BindAction(MoveRightAction, ETriggerEvent::Triggered, this, &ARigelPawn::MoveRight);
        EnhancedInputComponent->BindAction(KeyMoveForwardAction, ETriggerEvent::Triggered, this, &ARigelPawn::MoveForward_Key);
        EnhancedInputComponent->BindAction(KeyMoveRightAction, ETriggerEvent::Triggered, this, &ARigelPawn::MoveRight_Key);
        EnhancedInputComponent->BindAction(KeyUpDown, ETriggerEvent::Triggered, this, &ARigelPawn::MoveUp_Key);

    }
}

FVector ARigelPawn::PickLocation()
{
    FVector pickLocation = FVector::ZeroVector;;
    APlayerController* PlayerController = Cast<APlayerController>(Controller);
    if (PlayerController == nullptr) return pickLocation;

    FVector Location, Direction;
    if (PlayerController->DeprojectMousePositionToWorld(Location, Direction))
    {
        FHitResult result;

        GWorld->LineTraceSingleByChannel(result, Location, Direction * 10000000, ECC_Visibility);
        if (result.IsValidBlockingHit())
        {
            pickLocation = result.Location;
        }
        else
        {
            pickLocation = FVector::ZeroVector;
        }
    }
    return pickLocation;
}

AActor* ARigelPawn::PickActor()
{
    AActor* pickActor = nullptr;
    APlayerController* PlayerController = Cast<APlayerController>(Controller);
    if (PlayerController == nullptr) return pickActor;

    FVector Location, Direction;
    if (PlayerController->DeprojectMousePositionToWorld(Location, Direction))
    {
        FHitResult result;
        GWorld->LineTraceSingleByChannel(result, Location, Direction * 10000000, ECC_Visibility);
        if (result.IsValidBlockingHit())
        {
            OnPickEvent.Broadcast(result.GetActor(), result.GetComponent());
            auto pri = Cast<UPrimitiveComponent>(result.GetComponent());
            if (pri != nullptr)
            {
                //ARigelLevelEditor::RigelLevel()->AddSelect(pri);
            }
        }
    }
    return pickActor;
} 

UPrimitiveComponent* ARigelPawn::PickComponent()
{
    UPrimitiveComponent* pickPrimitive = nullptr;
    APlayerController* PlayerController = Cast<APlayerController>(Controller);
    if (PlayerController == nullptr) return nullptr;

    FVector Location, Direction;
    if (PlayerController->DeprojectMousePositionToWorld(Location, Direction))
    {
        FHitResult result;
        GWorld->LineTraceSingleByChannel(result, Location, Direction * 10000000, ECC_Visibility);
        if (result.IsValidBlockingHit())
        {
            pickPrimitive = result.GetComponent();
        }
    }
    return pickPrimitive;
}

void ARigelPawn::FlyToViewpoint(float time, const FViewpoint& Viewpoint)
{
    ViewpointComponent->RoamingToViewpoint(time, Viewpoint);
}

void ARigelPawn::FlyToActor(float time, const AActor* actor)
{

}

void ARigelPawn::Zoom(const FInputActionValue& Value)
{
    float delta = Value.Get<float>();
    
    FVector PivotPoint = PickLocation();
    FVector pawnLocation = GetActorLocation();
    //计算从点击位置到pawn位置的向量，Pcik位置是原点，指向pawn
    FVector PawnDir = pawnLocation - PivotPoint;
    double distance = PawnDir.Length();
    float speed = delta / 10.0;
    FVector2D aaa = FVector2D(distance, 0);
    //GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, TEXT("没有相交:") + aaa.ToString());
    FVector newLocation = UKismetMathLibrary::VLerp(pawnLocation, PivotPoint, speed);
    SetActorLocation(newLocation, false);
    if (FocusActor != nullptr)
    {
        FocusActor->SetActorHiddenInGame(false);
        FocusActor->SetActorLocation(PickWorldLocation);
    }
}

void ARigelPawn::YawRotation(const FInputActionValue& Value)
{
    if (!IsMouseRight)
    {
        return;
    }
    if (FocusActor != nullptr)
    {
        FocusActor->SetActorHiddenInGame(false);
        FocusActor->SetActorLocation(PickWorldLocation);
    }
    FVector2D delta = Value.Get<FVector2D>();
    float deltaAngle = delta.X * 3;
    
    //如果没有点击到具体坐标，就自传
    if (PickWorldLocation.Equals(FVector::ZeroVector))
    {
        FRotator rotateRotation = UKismetMathLibrary::ComposeRotators(GetActorRotation(), FRotator(0, deltaAngle, 0));
       
        if (rotateRotation.Pitch >= -85.0 && rotateRotation.Pitch <= 0)
        {
            FRotator newRotator = FRotator(rotateRotation.Pitch, rotateRotation.Yaw, 0.0);
            GetController()->SetControlRotation(newRotator);
        }
    }
    else
    {
        FVector pawnLocation = GetActorLocation();
        //计算从点击位置到pawn位置的向量，Pcik位置是原点，指向pawn
        FVector PawnDir = pawnLocation - PickWorldLocation;
        //将PawnDir旋转RotatorAngle度之后，的新的Pawn的位置
        FVector newLocation = UKismetMathLibrary::RotateAngleAxis(PawnDir, deltaAngle, FVector::UpVector);
        SetActorLocation(newLocation + PickWorldLocation, false);

        //计算pawn朝向
        FRotator rotatorPick = UKismetMathLibrary::RotatorFromAxisAndAngle(FVector::UpVector, deltaAngle);
        FRotator rotateRotation = UKismetMathLibrary::ComposeRotators(GetActorRotation(), rotatorPick);
        GetController()->SetControlRotation(rotateRotation);
    }
   
}

void ARigelPawn::PitchRotation(const FInputActionValue& Value)
{
    if (!IsMouseRight)
    {
        return;
    }
    if (FocusActor != nullptr)
    {
        FocusActor->SetActorHiddenInGame(false); 
        FocusActor->SetActorLocation(PickWorldLocation);
    }

    FVector2D delta = Value.Get<FVector2D>();
    float deltaAngle = delta.X * 3;

    FRotator ControllerRotator = GetController()->GetControlRotation();

    //如果没有点击到具体坐标，就自传
    if (PickWorldLocation.Equals(FVector::ZeroVector))
    {
        FRotator rotateRotation = UKismetMathLibrary::ComposeRotators(GetActorRotation(), FRotator(deltaAngle, 0, 0));
        //GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Green, rotateRotation.ToString());
        if (rotateRotation.Pitch >= -85.0 && rotateRotation.Pitch <= 0)
        {
            //这个地方yaw角度要用Controller，不能用计算的rotateRotation
            FRotator newRotator = FRotator(FMath::Clamp(rotateRotation.Pitch, -85.1, 0), ControllerRotator.Yaw, 0.0);
            GetController()->SetControlRotation(newRotator);
            
        }
    }
    else
    {
        FVector pawnLocation = GetActorLocation();
        //计算从点击位置到pawn位置的向量，Pcik位置是原点，指向pawn
        FVector PawnDir = pawnLocation - PickWorldLocation;

        //计算Pawn的角度
        
        FRotator rotatorPick = UKismetMathLibrary::RotatorFromAxisAndAngle(GetActorRightVector(), deltaAngle);
        FRotator rotateRotation = UKismetMathLibrary::ComposeRotators(GetActorRotation(), rotatorPick);
        
        if (rotateRotation.Pitch >= -85.0 && rotateRotation.Pitch <= 0)
        {
            FRotator newRotator = FRotator(FMath::Clamp(rotateRotation.Pitch, -85.0, 0), ControllerRotator.Yaw, 0.0);
            //将PawnDir旋转RotatorAngle度之后，的新的Pawn的位置
            FVector MovementVector = FRotationMatrix(ControllerRotator).GetScaledAxis(EAxis::Y);
            FVector newLocation = UKismetMathLibrary::RotateAngleAxis(PawnDir, deltaAngle, MovementVector);
            SetActorLocation(PickWorldLocation + newLocation);
            //GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Green, rotateRotation.ToString());
            GetController()->SetControlRotation(newRotator);
            
        }
    }
   
}

void ARigelPawn::Focus(const FInputActionValue& Value)
{

}

void ARigelPawn::OnLeftMousePressed(const FInputActionValue& Value)
{
    IsMouseLeft = true;
    PickWorldLocation = PickLocation();
    if (FocusActor != nullptr)
    {
        FocusActor->SetActorHiddenInGame(false);
        FocusActor->SetActorLocation(PickWorldLocation);
    }
}

void ARigelPawn::OnLeftMouseRelease(const FInputActionValue& Value)
{
    IsMouseLeft = false;
    Distance = 0.0;
    //鼠标按下的位置和鼠标释放的位置相近，视为点击否则是拖拽
    if (PickWorldLocation.Equals(PickLocation(), 0.0001))
    {
        PickActor();
    }
    
    if (FocusActor != nullptr)
    {
        FocusActor->SetActorHiddenInGame(true);
    }
}

void ARigelPawn::OnRightMousePressed(const FInputActionValue& Value)
{
    IsMouseRight = true;
    PickWorldLocation = PickLocation();
    if (FocusActor != nullptr)
    {
        FocusActor->SetActorHiddenInGame(false);
        FocusActor->SetActorLocation(PickWorldLocation);
    }
}

void ARigelPawn::OnRightMouseRelease(const FInputActionValue& Value)
{
    IsMouseRight = false;
    if (FocusActor != nullptr)
    {
        FocusActor->SetActorHiddenInGame(true);
    }
}

void ARigelPawn::MoveForward(const FInputActionValue& Value)
{
    if (IsMouseLeft)
    {
        FVector PawnLocation = GetActorLocation();
        FVector MoveDir = PickWorldLocation - PawnLocation;
        FVector CurrentMousrLocation = PickLocation();
        FVector moveVector = CurrentMousrLocation - PawnLocation;
        float speed = MoveDir.Z / 20;
        if (SpeedCurve != nullptr)
        {
            speed = SpeedCurve->GetFloatValue(PawnLocation.Z / 100.0);
        }
        FloatingMovement->MaxSpeed = speed;
        FVector2D delta = Value.Get<FVector2D>();

        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YanRotation(0, Rotation.Yaw, 0);

        const FVector ForwardDirection = FRotationMatrix(YanRotation).GetUnitAxis(EAxis::X);

        AddActorWorldOffset(ForwardDirection * delta.X * speed);
    }
    
} 

void ARigelPawn::MoveRight(const FInputActionValue& Value)
{
    if (IsMouseLeft)
    {
        FVector PawnLocation = GetActorLocation();
        FVector MoveDir = PickWorldLocation - PawnLocation;
        FVector CurrentMousrLocation = PickLocation();
        FVector moveVector = CurrentMousrLocation - PawnLocation;

        float speed = MoveDir.Z / 20;
        if (SpeedCurve != nullptr)
        {
            speed = SpeedCurve->GetFloatValue(PawnLocation.Z / 100.0);
        }

        FloatingMovement->MaxSpeed = speed;
        FVector2D delta = Value.Get<FVector2D>();

        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YanRotation(0, Rotation.Yaw, 0);

        const FVector RightDirection = FRotationMatrix(YanRotation).GetUnitAxis(EAxis::Y);

        AddActorWorldOffset(RightDirection * delta.X * speed * -1);
    }
}

void ARigelPawn::MoveForward_Key(const FInputActionValue& Value)
{
    FVector PawnLocation = GetActorLocation();
    float speed = PawnLocation.Z / 100;
    if (SpeedCurve != nullptr)
    {
        speed = SpeedCurve->GetFloatValue(PawnLocation.Z / 100.0);
    }
    FloatingMovement->MaxSpeed = speed;
    FVector2D delta = Value.Get<FVector2D>();

    const FRotator Rotation = Controller->GetControlRotation();
    const FRotator YanRotation(0, Rotation.Yaw, 0);

    const FVector ForwardDirection = FRotationMatrix(Rotation).GetUnitAxis(EAxis::X);
    AddActorWorldOffset(ForwardDirection * delta.X * speed);
}

void ARigelPawn::MoveRight_Key(const FInputActionValue& Value)
{
    FVector PawnLocation = GetActorLocation();
    float speed = PawnLocation.Z / 100;
    if (SpeedCurve != nullptr)
    {
        speed = SpeedCurve->GetFloatValue(PawnLocation.Z / 100.0);
    }
    FloatingMovement->MaxSpeed = speed;
    FVector2D delta = Value.Get<FVector2D>();

    const FRotator Rotation = Controller->GetControlRotation();
    const FRotator YanRotation(0, Rotation.Yaw, 0);

    const FVector RightDirection = FRotationMatrix(YanRotation).GetUnitAxis(EAxis::Y);
    AddActorWorldOffset(RightDirection * delta.X * speed);
}

void ARigelPawn::MoveUp_Key(const FInputActionValue& Value)
{
    FVector PawnLocation = GetActorLocation();
    float speed = PawnLocation.Z / 100;
    if (SpeedCurve != nullptr)
    {
        speed = SpeedCurve->GetFloatValue(PawnLocation.Z / 100.0);
    }
    FloatingMovement->MaxSpeed = speed;
    FVector2D delta = Value.Get<FVector2D>();

    AddActorWorldOffset(FVector(0, 0, delta.X * speed));
}

