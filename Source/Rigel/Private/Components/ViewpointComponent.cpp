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

}

// Called every frame
void UViewpointComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// ...
    if (Playing)
    {
        APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
        if (PlayerController == nullptr)
        {
            Duration = 0.0;
            Runtime = 0.0;
            Playing = false;
            return;
        }
        //如果动画时间为零，直接设置到终点
        if (Duration == 0.0)
        {
            Duration = 0.0;
            Runtime = 0.0;
            Playing = false;
            SetComponentTickEnabled(false);
            GetOwner()->SetActorLocation(TargetTransform.GetLocation());
            PlayerController->SetControlRotation(TargetTransform.Rotator());
            GetOwner()->SetActorTransform(TargetTransform);
        }
        else
        {
            Runtime += DeltaTime;
            float rotio = Runtime / Duration;
            //由于rotio基本不可能刚好到达模板位置，所以结束时，设置为TargetTransform
            if (rotio >= 1.0)
            {
                return;
                GetOwner()->SetActorLocation(TargetTransform.GetLocation());
                PlayerController->SetControlRotation(TargetTransform.Rotator());
                Duration = 0.0;
                Runtime = 0.0;
                Playing = false;
                SetComponentTickEnabled(false);
            }
            else
            {
                FTransform deltaTransform = UKismetMathLibrary::TLerp(StartTransform, TargetTransform, rotio, ELerpInterpolationMode::QuatInterp);

                FVector currentLocation = GetOwner()->GetActorLocation();
                FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(
                    currentLocation, TargetTransform.GetLocation());

                PlayerController->SetControlRotation(NewRotation);
                GetOwner()->SetActorLocation(deltaTransform.GetLocation());
            }
           

        }
    }
}

void UViewpointComponent::RoamingToViewpoint(float time, const FViewpoint& Viewpoint)
{
    AActor* owner = GetOwner();
    StartTransform = owner->GetTransform();
    Runtime = 0.0;
    Duration = time;
    TargetTransform.SetLocation(Viewpoint.Location);
    TargetTransform.SetRotation(FQuat::MakeFromRotator(Viewpoint.Rotation));
    Playing = true;
    SetComponentTickEnabled(true);
}

void UViewpointComponent::RoamingToActor(float time, const AActor* Actor)
{
    AActor* owner = GetOwner();
    StartTransform = owner->GetTransform();
    Runtime = 0.0;
    Duration = time;

    FVector Origin, BoxExtent;
    Actor->GetActorBounds(true, Origin, BoxExtent, true);
    
    // 计算合适的距离：基于包围盒的最大尺寸，乘以一个系数
    float CalculatedDistance = 3000; // 默认使用固定距离

    // 检查包围盒是否有效（如果所有维度都接近0，则认为无效）
    if (BoxExtent.SizeSquared() > 0.001f)
    {
        // 使用包围盒的最大维度乘以1.5倍作为距离（可根据需要调整系数）
        float MaxExtent = FMath::Max3(BoxExtent.X, BoxExtent.Y, BoxExtent.Z);
        CalculatedDistance = MaxExtent * 1.5f;

        // 确保距离不会太小
        CalculatedDistance = FMath::Max(CalculatedDistance, 100.0f);
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("Using fallback distance for actor %s"), *Actor->GetName());
    }

    TargetTransform.SetLocation(Actor->GetActorLocation());
    FVector TargetDirection = (TargetTransform.GetLocation() - Actor->GetActorLocation()).GetSafeNormal();
    if (TargetDirection.SizeSquared() > 0.001f)
    {
        FRotator NewRotation = TargetDirection.Rotation();
        TargetTransform.SetRotation(NewRotation.Quaternion());
    }
    Playing = true;
    SetComponentTickEnabled(true);
}
