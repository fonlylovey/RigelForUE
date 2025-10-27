#include "Actors/ViewpointManager.h"
#include "Components/ViewpointComponent.h"
#include "Components/SceneCaptureComponent2D.h"

#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Components/SceneComponent.h"

// Sets default values
AViewpointManager::AViewpointManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    RootComponent->SetMobility(EComponentMobility::Movable);

    Viewpoint = CreateDefaultSubobject<UViewpointComponent>(TEXT("Viewpoint"));

    SceneCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCapture"));
    SceneCapture->SetupAttachment(RootComponent);
    SceneCapture->CaptureSource = ESceneCaptureSource::SCS_SceneColorHDR;
        
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
    if (!IsValid(viewpoint.Thumbnail))
    {
        FString ProjectDir = FPaths::ProjectDir();
        FString DataPath = ProjectDir + TEXT("Viewpoint/");
        FString filePath = DataPath + viewpoint.ID + ".png";
        UKismetRenderingLibrary::ExportRenderTarget(this, SceneCapture->TextureTarget, DataPath, viewpoint.ID + ".png");
        viewpoint.Thumbnail = UKismetRenderingLibrary::ImportFileAsTexture2D(this, filePath);
    }
    
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
    if (ViewpointList.Contains(ViewpointID))
    {
        FViewpoint viewpoint = ViewpointList.FindRef(ViewpointID);
        return viewpoint;
    }
   
    return FViewpoint();
}
