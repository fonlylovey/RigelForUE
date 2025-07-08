#include "Actors/ViewpointManager.h"
#include "Components/ViewpointComponent.h"
#include "Components/SceneCaptureComponent2D.h"

// Sets default values
AViewpointManager::AViewpointManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    RootComponent->SetMobility(EComponentMobility::Movable);

    Viewpoint = CreateDefaultSubobject<UViewpointComponent>(TEXT("Viewpoint"));

    RenderTarget = CreateDefaultSubobject<UTextureRenderTarget2D>(TEXT("RenderTarget"));
    
    SceneCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCapture"));
    SceneCapture->SetupAttachment(RootComponent);
    SceneCapture->TextureTarget = RenderTarget;
    SceneCapture->CaptureSource = SCS_FinalColorLDR;

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetMobility(EComponentMobility::Movable);
    Camera->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AViewpointManager::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AViewpointManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AViewpointManager::AddViewpoint()
{
    FViewpoint viewpoint;
    viewpoint.Location = GetOwner()->GetActorLocation();
    viewpoint.Rotation = GetOwner()->GetActorRotation();
    viewpoint.ID = MakeUniqueObjectName(this, UViewpointComponent::StaticClass(), TEXT("Viewpoint_")).ToString();
    viewpoint.Name = viewpoint.ID;
    SceneCapture->CaptureScene();
    viewpoint.Thumbnail = RenderTarget;
    ViewpointList.Add(viewpoint.ID, viewpoint);
    ViewpointList->AddViewpoint();
}