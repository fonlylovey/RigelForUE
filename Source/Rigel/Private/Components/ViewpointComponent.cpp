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
    FVector OffsetHorizontalDir = TargetForward.RotateAngleAxis(15.0, FVector::UpVector);
    OffsetHorizontalDir.Normalize();
   
    FVector NewPawnLocation = Actor->GetActorLocation() - (TargetForward * 100000);
    NewPawnLocation.Z = CalculatedDistance;

    FVector DirectionToTarget = (Actor->GetActorLocation() - NewPawnLocation).GetSafeNormal();
    FRotator NewPawnRotation = DirectionToTarget.Rotation(); // 转为旋转
    NewPawnRotation.Yaw -= 15.0;
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

void UViewpointComponent::RoamingToLocation(FVector Location, FVector Offset, int Height, float Time)
{
    AActor* owner = GetOwner();
    if (!owner || !PlayerController)
    {
        UE_LOG(LogTemp, Warning, TEXT("FlyToUE: Owner or PlayerController is null!"));
        return;
    }

    // 1. 记录起始变换（与RoamingToUE保持一致）
    StartTransform = owner->GetTransform();

    // 2. 计算相机最终位置：以目标位置为基准，保持高度距离 + 水平偏移
    // 核心逻辑：相机在目标位置的水平平面（Z轴对齐）上方keepHeight距离处，叠加水平偏移
    FVector finalCameraLocation = Location;
    finalCameraLocation.Z += Height; // 保持与目标位置的高度差（垂直距离）
    finalCameraLocation += Offset; // 水平方向偏移（左右/前后，不影响高度）

    // 3. 计算相机朝向：始终指向目标位置（虚拟Actor的中心）
    FRotator lookAtRotation = (Location - finalCameraLocation).Rotation();
    // 限制俯仰角（避免相机翻转，确保自然视角）
    lookAtRotation.Pitch = FMath::Clamp(lookAtRotation.Pitch, -85.f, -5.f); // 建议俯视角度（可调整范围）
    lookAtRotation.Roll = 0.0f; // 强制水平翻滚，避免倾斜

    // 4. 设置目标变换（与RoamingToUE结构完全一致）
    TargetTransform.SetLocation(finalCameraLocation);
    TargetTransform.SetRotation(FQuat::MakeFromRotator(lookAtRotation));
    Duration = Time;

    // 5. 复用RoamingToUE的动画控制逻辑
    if (Duration <= 0.0f)
    {
        // 无动画时间，直接跳转到位
        Runtime = 0.0f;
        Playing = false;
        SetComponentTickEnabled(false);

        PlayerController->SetControlRotation(lookAtRotation);
        GetOwner()->SetActorTransform(TargetTransform);
    }
    else
    {
        // 启动平滑飞行动画（Tick驱动插值）
        Runtime = 0.0f;
        Playing = true;
        SetComponentTickEnabled(true);
    }
}
