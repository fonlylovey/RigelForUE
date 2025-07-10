#include "Actors/ViewpointManager.h"
#include "Components/ViewpointComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AViewpointManager::AViewpointManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    RootComponent->SetMobility(EComponentMobility::Movable);

    Viewpoint = CreateDefaultSubobject<UViewpointComponent>(TEXT("Viewpoint"));

    RenderTarget = CreateDefaultSubobject<UTextureRenderTarget2D>(TEXT("RenderTarget"));
    RenderTarget->ResizeTarget(128, 128);
    RenderTarget->UpdateResource();

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
    viewpoint.Location = GetActorLocation();
    viewpoint.Rotation = GetActorRotation();
    viewpoint.ID = MakeUniqueObjectName(this, UViewpointComponent::StaticClass(), TEXT("Viewpoint")).ToString();
    viewpoint.Name = viewpoint.ID;
    SceneCapture->CaptureScene();
    viewpoint.Thumbnail = RenderTarget->ConstructTexture2D(GetTransientPackage(), viewpoint.Name, RenderTarget->GetMaskedFlags());
    viewpoint.Thumbnail->UpdateResource();
    ViewpointList.Add(viewpoint.ID, viewpoint);
}

AViewpointManager* AViewpointManager::GetViewpointManager()
{
    AViewpointManager* manager = nullptr;
    if (GWorld)
    {
        for (TActorIterator<AActor> Iter(GWorld, AViewpointManager::StaticClass()); Iter; ++Iter)
        {
            AActor* actor = *Iter;
            if (actor->GetLevel() == GWorld->PersistentLevel) {
                manager = Cast<AViewpointManager>(actor);
                break;
            }
        }
    }
    else
    {
        manager = GWorld->SpawnActor<AViewpointManager>();
    }
    return manager;
}

FViewpoint AViewpointManager::GetViewpoint(const FString& ViewpointID)
{
    FViewpoint viewpoint = ViewpointList.FindRef(ViewpointID);
    return viewpoint;
}
