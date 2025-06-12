#include "Gameplay/RigelPawn.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "Components/WidgetInteractionComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/FloatingPawnMovement.h"

// Sets default values
ARigelPawn::ARigelPawn(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    RootComponent = CreateDefaultSubobject<USceneComponent>("RootComponent");
    Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
    WidgetInteraction = CreateDefaultSubobject<UWidgetInteractionComponent>("WidgetInteraction");
    PivotMesh = CreateDefaultSubobject<UStaticMeshComponent>("PivotMesh");
    PivotMesh->SetVisibility(false);
    PivotMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    FloatingMovement = CreateDefaultSubobject<UFloatingPawnMovement>("FloatingMovement");
    FloatingMovement->UpdatedComponent = RootComponent;
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
        EnhancedInputComponent->BindAction(PanAction, ETriggerEvent::Triggered, this, &ARigelPawn::Pan);
        EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &ARigelPawn::Zoom);
        EnhancedInputComponent->BindAction(YawRotationAction, ETriggerEvent::Triggered, this, &ARigelPawn::YawRotation);
        EnhancedInputComponent->BindAction(PitchRotationAction, ETriggerEvent::Triggered, this, &ARigelPawn::PitchRotation);
        EnhancedInputComponent->BindAction(LeftMousePressedAction, ETriggerEvent::Triggered, this, &ARigelPawn::OnLeftMousePressed);
        EnhancedInputComponent->BindAction(LeftMouseReleaseAction, ETriggerEvent::Triggered, this, &ARigelPawn::OnLeftMouseRelease);
        EnhancedInputComponent->BindAction(RightMousePressedAction, ETriggerEvent::Triggered, this, &ARigelPawn::OnRightMousePressed);
        EnhancedInputComponent->BindAction(RightMouseReleaseAction, ETriggerEvent::Triggered, this, &ARigelPawn::OnRightMouseRelease);

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
        GWorld->LineTraceSingleByChannel(result, Location, Direction * 10000000000, ECC_Visibility);
        if (result.IsValidBlockingHit())
        {
            pickLocation = result.Location;
        }
        else
        {
            //GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, TEXT("没有相交:") + FoucsLocation.ToString());
            pickLocation = Location + Direction * 1000;
        }
    }
    return pickLocation;
}

void ARigelPawn::Pan(const FInputActionValue& Value)
{
    if (!IsMouseLeft)
    {
        return;
    }
    FVector MoveDir = PickWorldLocation - GetActorLocation();
    if (Distance == 0.0)
    {
        Distance = MoveDir.Length();
    }
    FVector CurrentMousrLocation = PickLocation();
    FVector moveVector = CurrentMousrLocation - GetActorLocation();
    float realDis = moveVector.Length();
    float speed = MoveDir.Z / 20;
    FloatingMovement->MaxSpeed = speed;
    FVector2D delta = Value.Get<FVector2D>();
    
    const FRotator Rotation = Controller->GetControlRotation();
    const FRotator YanRotation(0, Rotation.Yaw, 0);

    const FVector ForwardDirection = FRotationMatrix(YanRotation).GetUnitAxis(EAxis::X);

    const FVector RightDirection = FRotationMatrix(YanRotation).GetUnitAxis(EAxis::Y);

    AddActorWorldOffset(ForwardDirection * delta.Y * speed);
    AddActorWorldOffset(RightDirection * delta.X * speed);
    /*
    if (delta.X > 0)
    {
        MoveForward(delta.X);
    }
   
    if (delta.Y > 0)
    
        //MoveRight(delta.Y);
    }
   */
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
}

void ARigelPawn::YawRotation(const FInputActionValue& Value)
{
    if (!IsMouseRight)
    {
        return;
    }
    PivotMesh->SetVisibility(true);
    PivotMesh->SetWorldLocation(PickWorldLocation);
    FVector2D delta = Value.Get<FVector2D>();
    float deltaAngle = delta.X * 3;
    FVector pawnLocation = GetActorLocation();
    //计算从点击位置到pawn位置的向量，Pcik位置是原点，指向pawn
    FVector PawnDir = pawnLocation - PickWorldLocation;

    //将PawnDir旋转RotatorAngle度之后，的新的Pawn的位置
    FVector newLocation = UKismetMathLibrary::RotateAngleAxis(PawnDir, deltaAngle, FVector::ZAxisVector);
    SetActorLocation(PickWorldLocation + newLocation, true);

    FRotator rotatorPick = UKismetMathLibrary::RotatorFromAxisAndAngle(FVector::ZAxisVector, deltaAngle);
    FRotator rotateRotation = UKismetMathLibrary::ComposeRotators(GetController()->GetControlRotation(), rotatorPick);
    GetController()->SetControlRotation(rotateRotation);
    //GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, "Horizontal:" + FString::FormatAsNumber(rotateRotation.Pitch));
}

void ARigelPawn::PitchRotation(const FInputActionValue& Value)
{
    if (!IsMouseRight)
    {
        return;
    }
    PivotMesh->SetVisibility(true);
    PivotMesh->SetWorldLocation(PickWorldLocation);
    FVector2D delta = Value.Get<FVector2D>();
    float deltaAngle = delta.Y * 3;

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
        //GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, "Vertical:" + FString::FormatAsNumber(rotateRotation.Pitch));
    }
}

void ARigelPawn::Focus(const FInputActionValue& Value)
{

}

void ARigelPawn::OnLeftMousePressed(const FInputActionValue& Value)
{
    IsMouseLeft = true;
    PickWorldLocation = PickLocation();
    PivotMesh->SetVisibility(true);
    PivotMesh->SetWorldLocation(PickWorldLocation);
}

void ARigelPawn::OnLeftMouseRelease(const FInputActionValue& Value)
{
    IsMouseLeft = false;
    Distance = 0.0;
    PivotMesh->SetVisibility(false);
}

void ARigelPawn::OnRightMousePressed(const FInputActionValue& Value)
{
    IsMouseRight = true;
    PickWorldLocation = PickLocation();
    PivotMesh->SetVisibility(true);
    PivotMesh->SetWorldLocation(PickWorldLocation);
}

void ARigelPawn::OnRightMouseRelease(const FInputActionValue& Value)
{
    IsMouseRight = false;
    PivotMesh->SetVisibility(false);
}

void ARigelPawn::MoveForward(const float vlaue)
{
    FVector MoveDir = PickWorldLocation - GetActorLocation();
    if (Distance == 0.0)
    {
        GEngine->AddOnScreenDebugMessage(-1, 20, FColor::Green, "Pick:" + PickWorldLocation.ToString());
        Distance = MoveDir.Length();
    }
    MoveDir.Normalize();
    FVector CurrentMousrLocation = PickLocation();
    FVector moveVector = CurrentMousrLocation - GetActorLocation();
    float realDis = moveVector.Length();
    float speed = (realDis / Distance) * 50000;
    FVector deltaMove = MoveDir * vlaue * -1 * speed;
    GEngine->AddOnScreenDebugMessage(-1, 20, FColor::Green, "speed:" + FString::FormatAsNumber(speed));
    //计算从点击位置到pawn位置的向量，Pcik位置是原点，指向pawnFString::FormatAsNumber(realDis)
    AddActorWorldOffset(deltaMove);
    //计算高度差
    //SetActorLocation(newLocation, true);
}

void ARigelPawn::MoveRight(const float vlaue)
{
    FVector MoveDir = PickWorldLocation - GetActorLocation();
    if (Distance == 0.0)
    {
        GEngine->AddOnScreenDebugMessage(-1, 20, FColor::Green, "Pick:" + PickWorldLocation.ToString());
        Distance = MoveDir.Length();
    }
    MoveDir.Normalize();
    FVector CurrentMousrLocation = PickLocation();
    FVector moveVector = CurrentMousrLocation - GetActorLocation();
    float realDis = moveVector.Length();
    float speed = (realDis / Distance) * 1000;
    GEngine->AddOnScreenDebugMessage(-1, 20, FColor::Green, "speed:" + FString::FormatAsNumber(speed));
    FVector deltaMove = MoveDir.RightVector * vlaue * speed;

    //计算从点击位置到pawn位置的向量，Pcik位置是原点，指向pawnFString::FormatAsNumber(realDis)
    AddActorWorldOffset(deltaMove);
    //计算高度差
    //SetActorLocation(newLocation, true);
}

