#include "Components/ViewpointComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UViewpointComponent::UViewpointComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
    SetComponentTickEnabled(false);
}

// Called when the game starts
void UViewpointComponent::BeginPlay()
{
	Super::BeginPlay();
	// ...
    PlayerController = UGameplayStatics::GetPlayerController(this, 0);
    check(PlayerController != nullptr);
}

// Called every frame
void UViewpointComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// ...
    if (Playing)
    {
        if (PlayerController == nullptr)
        {
            Duration = 0.0;
            Runtime = 0.0;
            Playing = false;
            return;
        }
       
        {
            Runtime += DeltaTime;
            float rotio = Runtime / Duration;
            FTransform deltaTransform = UKismetMathLibrary::TLerp(StartTransform, TargetTransform, rotio, ELerpInterpolationMode::QuatInterp);
            //由于rotio基本不可能刚好到达模板位置，所以结束时，设置为TargetTransform
            if (rotio >= 1.0)
            {
                Duration = 0.0;
                Runtime = 0.0;
                Playing = false;
                SetComponentTickEnabled(false);
                FRotator rotator = TargetTransform.Rotator();
                GetOwner()->SetActorTransform(TargetTransform);
                PlayerController->SetControlRotation(TargetTransform.Rotator());
            }
            else
            {
                float distance = FVector::Distance(StartTransform.GetLocation(), TargetTransform.GetLocation());
                if (TargetTransform.GetLocation().Z > 300000)
                {
                    distance = 1;
                }
                float height = UKismetMathLibrary::Sin(rotio * 3.14);

                deltaTransform.SetLocation(deltaTransform.GetLocation() );
                GetOwner()->SetActorTransform(deltaTransform);
                PlayerController->SetControlRotation(deltaTransform.Rotator());
                //FString::Printf(TEXT("%f"),+ FVector(0, 0, height * distance)
                //GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green,  deltaTransform.Rotator().ToString());
            }
           

        }
    }
}

void UViewpointComponent::RoamingToViewpoint(float time, const FViewpoint& Viewpoint)
{
   
    AActor* owner = GetOwner();
    StartTransform = owner->GetTransform();
    TargetTransform.SetLocation(Viewpoint.Location);
    TargetTransform.SetRotation(FQuat::MakeFromRotator(Viewpoint.Rotation));
    FRotator targetRotator = TargetTransform.Rotator();
    Duration = time;
    //如果动画时间为零，直接设置到终点
    if (Duration == 0.0)
    {
        Runtime = 0.0;
        Playing = false;
        SetComponentTickEnabled(false);
        
        PlayerController->SetControlRotation(targetRotator);
        GetOwner()->SetActorTransform(TargetTransform);
    }
    else
    {
        Runtime = 0.0;
        Playing = true;
        SetComponentTickEnabled(true);
    }
}

void UViewpointComponent::RoamingToActor(float time, const AActor* Actor)
{
    AActor* owner = GetOwner();
    StartTransform = owner->GetTransform();

    FVector Origin, BoxExtent;
    Actor->GetActorBounds(true, Origin, BoxExtent, true);
    
    // 计算合适的距离：基于包围盒的最大尺寸，乘以一个系数
    float CalculatedDistance = Origin.Distance(Origin, BoxExtent); // 默认使用固定距离

    // 检查包围盒是否有效（如果所有维度都接近0，则认为无效）
    if (!BoxExtent.ContainsNaN())
    {
        // 使用包围盒的最大维度乘以1.5倍作为距离（可根据需要调整系数）
        float MaxExtent = FMath::Max3(BoxExtent.X, BoxExtent.Y, BoxExtent.Z);
        CalculatedDistance = MaxExtent * 1.5f;

        // 确保距离不会太小
        CalculatedDistance = FMath::Max(CalculatedDistance * 0.2, 100.0f);
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("Using fallback distance for actor %s"), *Actor->GetName());
    }
    FVector TargetForward = Actor->GetActorForwardVector();
    //dir.X = 0;
    //dir.Y = 0;
    //dir.Z = 0;
    FVector NewPawnLocation = Actor->GetActorLocation() - (TargetForward * 100000);
    NewPawnLocation.Z = CalculatedDistance;
    FVector DirectionToTarget = (Actor->GetActorLocation() - NewPawnLocation).GetSafeNormal();
    //FRotator lookAt = UKismetMathLibrary::FindLookAtRotation(owner->GetActorLocation(), target);
    FRotator NewPawnRotation = DirectionToTarget.Rotation(); // 转为旋转
    
    TargetTransform.SetLocation(NewPawnLocation);
    TargetTransform.SetRotation(NewPawnRotation.Quaternion());
    
    Runtime = 0.0;
    Duration = time;
    Playing = true;
    SetComponentTickEnabled(true);
}

void UViewpointComponent::RoamingToUE(float time, const FVector& Location, const FRotator& Rotation)
{
    AActor* owner = GetOwner();
    StartTransform = owner->GetTransform();
    TargetTransform.SetLocation(Location);
    TargetTransform.SetRotation(FQuat::MakeFromRotator(Rotation));
    FRotator targetRotator = TargetTransform.Rotator();
    Duration = time;
    //如果动画时间为零，直接设置到终点
    if (Duration == 0.0)
    {
        Runtime = 0.0;
        Playing = false;
        SetComponentTickEnabled(false);

        PlayerController->SetControlRotation(targetRotator);
        GetOwner()->SetActorTransform(TargetTransform);
    }
    else
    {
        Runtime = 0.0;
        Playing = true;
        SetComponentTickEnabled(true);
    }
}

