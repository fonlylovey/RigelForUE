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
    Viewpoint = CreateDefaultSubobject<UViewpointComponent>("Viewpoint");
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
        GWorld->LineTraceSingleByChannel(result, Location, Direction * 1000000, ECC_Visibility);
        if (result.IsValidBlockingHit())
        {
            pickLocation = result.Location;
        }
        else
        {
            //GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, TEXT("没有相交:") + FoucsLocation.ToString());
            pickLocation = FVector::ZeroVector;
        }
    }
    return pickLocation;
}

AActor* ARigelPawn::PickActor()
{
    AActor* pickActor = nullptr;
    APlayerController* PlayerController = Cast<APlayerController>(Controller);
    if (PlayerController == nullptr) return nullptr;

    FVector Location, Direction;
    if (PlayerController->DeprojectMousePositionToWorld(Location, Direction))
    {
        FHitResult result;
        GWorld->LineTraceSingleByChannel(result, Location, Direction * 10000000000, ECC_Visibility);
        if (result.IsValidBlockingHit())
        {
            pickActor = result.GetActor();
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

void ARigelPawn::AddViewpoint()
{
    Viewpoint->AddViewpoint();
}

void ARigelPawn::FlyToViewpoint(float time, const FString& ID)
{
    Viewpoint->RoamingToViewpoint(time, ID);
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
    FVector newLocation = UKismetMathLibrary::VLerp(pawnLocation, PivotPoint, speed);
    SetActorLocation(newLocation, true);
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
        FRotator rotateRotation = UKismetMathLibrary::ComposeRotators(GetController()->GetControlRotation(), FRotator(deltaAngle, 0, 0));
        if (rotateRotation.Pitch >= -85.0 && rotateRotation.Pitch <= 0)
        {
            FRotator newRotator = FRotator(FMath::Clamp(rotateRotation.Pitch, -85.1, 0), rotateRotation.Yaw, 0.0);
            //GetController()->SetControlRotation(newRotator);
        }
    }
    else
    {
        FVector pawnLocation = GetActorLocation();
        //计算从点击位置到pawn位置的向量，Pcik位置是原点，指向pawn
        FVector PawnDir = pawnLocation - PickWorldLocation;
        PawnDir.Normalize();
        //将PawnDir旋转RotatorAngle度之后，的新的Pawn的位置
        FVector newLocation = UKismetMathLibrary::RotateAngleAxis(PawnDir, deltaAngle, FVector::UpVector);
        SetActorLocation(newLocation + PickWorldLocation, false);

        //计算pawn朝向
        FRotator rotatorPick = UKismetMathLibrary::RotatorFromAxisAndAngle(FVector::UpVector, deltaAngle);
        FRotator rotateRotation = UKismetMathLibrary::ComposeRotators(GetController()->GetControlRotation(), rotatorPick);
        GetController()->SetControlRotation(rotateRotation);
        GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, "Horizontal:" + PickWorldLocation.ToString());
    }
   
}

void ARigelPawn::PitchRotation(const FInputActionValue& Value)
{
    return;
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
        FRotator rotatorPick = UKismetMathLibrary::RotatorFromAxisAndAngle(GetActorRightVector(), deltaAngle);
        FRotator rotateRotation = UKismetMathLibrary::ComposeRotators(GetController()->GetControlRotation(), rotatorPick);
        if (rotateRotation.Pitch >= -85.0 && rotateRotation.Pitch <= 0)
        {
            FRotator newRotator = FRotator(FMath::Clamp(rotateRotation.Pitch, -85.1, 0), rotateRotation.Yaw, 0.0);
            GetController()->SetControlRotation(newRotator);
            SetActorLocation(PickWorldLocation, false);
        }
    }
    else
    {
        FVector pawnLocation = GetActorLocation();
        //计算从点击位置到pawn位置的向量，Pcik位置是原点，指向pawn
        FVector PawnDir = pawnLocation - PickWorldLocation;

        //将PawnDir旋转RotatorAngle度之后，的新的Pawn的位置
        FRotator const ControlSpaceRot = this->GetViewRotation();
        FVector MovementVector = FRotationMatrix(ControlSpaceRot).GetScaledAxis(EAxis::Y);
        FVector newLocation = UKismetMathLibrary::RotateAngleAxis(PawnDir, deltaAngle, MovementVector);

        AActor* ControllerActor = GetController();

        //计算Pawn的角度
        FRotator rotatorPick = UKismetMathLibrary::RotatorFromAxisAndAngle(ControllerActor->GetActorRightVector(), deltaAngle);
        FRotator rotateRotation = UKismetMathLibrary::ComposeRotators(GetController()->GetControlRotation(), rotatorPick);
        if (rotateRotation.Pitch >= -85.0 && rotateRotation.Pitch <= 0)
        {
            FRotator newRotator = FRotator(FMath::Clamp(rotateRotation.Pitch, -85.1, 0), rotateRotation.Yaw, 0.0);
            GetController()->SetControlRotation(newRotator);
            SetActorLocation(PickWorldLocation + newLocation, false);
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
    GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, TEXT("AAA"));
}

void ARigelPawn::OnLeftMouseRelease(const FInputActionValue& Value)
{
    IsMouseLeft = false;
    Distance = 0.0;
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
    FVector MoveDir = PickWorldLocation - GetActorLocation();
    FVector CurrentMousrLocation = PickLocation();
    FVector moveVector = CurrentMousrLocation - GetActorLocation();
    float speed = MoveDir.Z / 20;
    FloatingMovement->MaxSpeed = speed;
    FVector2D delta = Value.Get<FVector2D>();

    const FRotator Rotation = Controller->GetControlRotation();
    const FRotator YanRotation(0, Rotation.Yaw, 0);

    const FVector ForwardDirection = FRotationMatrix(YanRotation).GetUnitAxis(EAxis::X);

    AddActorWorldOffset(ForwardDirection * delta.X * speed * -1);
}

void ARigelPawn::MoveRight(const FInputActionValue& Value)
{
    FVector MoveDir = PickWorldLocation - GetActorLocation();
    FVector CurrentMousrLocation = PickLocation();
    FVector moveVector = CurrentMousrLocation - GetActorLocation();
    float speed = MoveDir.Z / 20;
    FloatingMovement->MaxSpeed = speed;
    FVector2D delta = Value.Get<FVector2D>();

    const FRotator Rotation = Controller->GetControlRotation();
    const FRotator YanRotation(0, Rotation.Yaw, 0);

    const FVector RightDirection = FRotationMatrix(YanRotation).GetUnitAxis(EAxis::Y);

    AddActorWorldOffset(RightDirection * delta.X * speed);
}

