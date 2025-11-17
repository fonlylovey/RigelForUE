#include "WebAPI/RigelLevelEditor.h"

#include "EngineUtils.h"
#undef OPAQUE

#include "Cesium3DTileset.h"
#include "WebAPI/WebWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Gameplay/RigelPawn.h"
#include "WebAPI/RigelAPISubsystem.h"
#include "Builder/SplinePathMesh.h"
#include "GeoReferencingSystem.h"
#include "Actors/POIBase.h"
#include "Gameplay/RigelPlayerController.h"
#include "CesiumWebMapServiceRasterOverlay.h"
#include "CesiumWebMapTileServiceRasterOverlay.h"
#include "Components/SplineComponent.h"

// Sets default values
ARigelLevelEditor::ARigelLevelEditor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}
// Called when the game starts or when spawned
void ARigelLevelEditor::BeginPlay()
{
    Super::BeginPlay();
    InitOption();
}

// Called every frame
void ARigelLevelEditor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

void ARigelLevelEditor::OnConstruction(const FTransform& Transform)
{
    if (GeoReferencing == nullptr)
    {
        if (GWorld)
        {
            for (TActorIterator<AActor> Iter(GWorld, AGeoReferencingSystem::StaticClass()); Iter; ++Iter)
            {
                AActor* actor = *Iter;
                if (actor->GetLevel() == GWorld->PersistentLevel) {
                    GeoReferencing = Cast<AGeoReferencingSystem>(actor);
                    break;
                }
            }
        }
    }
}

ARigelLevelEditor* ARigelLevelEditor::RigelLevel()
{
	ARigelLevelEditor* manager = nullptr;
	if (GWorld)
	{
		for (TActorIterator<AActor> Iter(GWorld, ARigelLevelEditor::StaticClass()); Iter; ++Iter)
		{
			AActor* actor = *Iter;
			if (actor->GetLevel() == GWorld->PersistentLevel) {
				manager = Cast<ARigelLevelEditor>(actor);
				break;
			}
		}
	}
	else
	{
		manager = GWorld->SpawnActor<ARigelLevelEditor>();
	}
	return manager;
}

void ARigelLevelEditor::InitOption()
{
    FString ProjectDir = FPaths::ProjectDir();
    FString ConfigPath = ProjectDir + TEXT("RigelGame.ini");
    GConfig->LoadFile(ConfigPath);
    GConfig->GetString(TEXT("WEB"), TEXT("HtmlUrl"), HtmlURL, ConfigPath);
    GConfig->GetString(TEXT("GISServer"), TEXT("IP"), GISServerIP, ConfigPath);
    GConfig->GetInt(TEXT("StreamMode"), TEXT("StreamMode"), StreamMode, ConfigPath);
    GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, FString::FromInt(StreamMode));
}

AActor* ARigelLevelEditor::GetActor(const FString& Name)
{
    AActor* actor = ActorMap.FindRef(Name);
    if (!IsValid(actor))
    {
        actor = RuntimeMap.FindRef(Name);
    }
    if (!IsValid(actor))
    {
        actor = LayerMap.FindRef(Name);
    }
    return  actor;
}

AActor* ARigelLevelEditor::GetRuntimeActor(const FString& Name)
{
    AActor* actor = RuntimeMap.FindRef(Name);
    if (!IsValid(actor))
    {
        actor = RuntimeMap.FindRef(Name);
    }
    return  actor;
}

void ARigelLevelEditor::RemoveRuntimeActor(const FString& Name)
{
    AActor* actor = RuntimeMap.FindRef(Name);
    if (!IsValid(actor))
    {
        RuntimeMap.Remove(Name);
        GetWorld()->DestroyActor(actor);
    }
}
AActor* ARigelLevelEditor::FindActor(const FString& Name)
{
    AActor* actor = GetActor(Name);
    
    if (actor != nullptr)
   {
       return actor;
   }
	if (GetWorld() != nullptr)
	{
        for (TActorIterator<AActor> Iter(GWorld, AActor::StaticClass()); Iter; ++Iter)
        {
            AActor* theActor = *Iter;
            FString strName = theActor->GetActorNameOrLabel();
            if (strName.Contains(Name))
            {
                actor = theActor;
                break;
            }
        }
	}
    return actor;
}

void ARigelLevelEditor::AddActorToActorMap(const FString& Name, AActor* Actor)
{
    if (!Name.IsEmpty() && IsValid(Actor))
    {
        ActorMap.Add(Name, Actor);
    }
}

void ARigelLevelEditor::AddActorToLayerMap(const FString& Name, AActor* Actor)
{
    if (!Name.IsEmpty() && IsValid(Actor))
    {
        LayerMap.Add(Name, Actor);
    }
}

void ARigelLevelEditor::SetLayerVisibility(const FString& LayerID, bool isVisible)
{
    AActor* layer = LayerMap.FindRef(LayerID);
    if (layer != nullptr)
    {
        layer->SetActorHiddenInGame(!isVisible);
    }
}

void ARigelLevelEditor::Add3DTiles(const FString& LayerID, const FString& URL, float Height)
{
    if (GetWorld() != nullptr)
    {
        FActorSpawnParameters spawnPara;
        spawnPara.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        ACesium3DTileset* tileset = GetWorld()->SpawnActor<ACesium3DTileset>(
            FVector(0, 0, Height), FRotator::ZeroRotator, spawnPara);
        tileset->SetTilesetSource(ETilesetSource::FromUrl);
        tileset->SetUrl(URL);
        tileset->RefreshTileset();
        RuntimeMap.Add(LayerID, tileset);
    }
}

void ARigelLevelEditor::RemoveLayer(const FString& LayerID)
{
    AActor* actor = RuntimeMap.FindRef(LayerID);
    if (actor != nullptr)
    {
        actor->SetActorHiddenInGame(true);
        RuntimeMap.Remove(LayerID);
    }
}

void ARigelLevelEditor::AddSplineMesh()
{

}

void ARigelLevelEditor::AddSelect(UPrimitiveComponent* component)
{
    if (component != nullptr)
    {
        SelectSet.Add(component);
        component->SetMaterial(0, SelectMaterial);
    }
}

void ARigelLevelEditor::ClearSelect()
{

}

void ARigelLevelEditor::SendMessageToWeb(const FString& Function, const FJsonLibraryValue& Data)
{
#if WITH_EDITOR

    if (WebWidget != nullptr)
    {
        WebWidget->SendMessage(Function, Data);
    }
#endif

    ARigelPlayerController* rigelController = Cast<ARigelPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
    if (IsValid(rigelController))
    {
        rigelController->SendMessageToWeb(Data.Stringify());
    }
}

void ARigelLevelEditor::SetWebWIdget(UWebWidget* widget)
{
    WebWidget = widget;
}

void ARigelLevelEditor::StartDrawGeometry(const FString& ActorName)
{
    ARigelPawn* rigelPawn = Cast<ARigelPawn>(UGameplayStatics::GetPlayerPawn(GWorld, 0));
    if (rigelPawn != nullptr)
    {
        rigelPawn->OnPickLocation.AddDynamic(this, &ARigelLevelEditor::OnDrawing);
    }
    CreateSplineActor(ActorName);
}

void ARigelLevelEditor::EndDrawGeometry()
{
    ARigelPawn* rigelPawn = Cast<ARigelPawn>(UGameplayStatics::GetPlayerPawn(GWorld, 0));
    if (rigelPawn != nullptr)
    {
        rigelPawn->OnPickLocation.RemoveDynamic(this, &ARigelLevelEditor::OnDrawing);
    }
}

void ARigelLevelEditor::OnDrawing(const FVector& Location)
{
    if (DrawingSplineActor != nullptr)
    {

        int32 pointCount = DrawingSplineActor->Spline->GetNumberOfSplinePoints();
        if (pointCount == 0)
        {
            //DrawingSplineActor->SetActorLocation(Location);
        }
        DrawingSplineActor->AddPoint(Location);
        DrawingSplineActor->UpdateSplineMesh();
        DrawingSplineActor->Spline->SetSplinePointType(pointCount, ESplinePointType::Linear);
    }
    
}

ASplinePathMesh* ARigelLevelEditor::CreateSplineActor(const FString& ActorName)
{
    if (GetWorld() != nullptr)
    {
        FActorSpawnParameters spawnPara;
        spawnPara.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        FString strPath = TEXT("/Rigel/Blueprints/BaseClass/BP_DrawingSplineMesh.BP_DrawingSplineMesh_C");

        UClass* SplineClass = LoadClass<ASplinePathMesh>(nullptr, *strPath);
        ASplinePathMesh* SplineActor = GetWorld()->SpawnActor<ASplinePathMesh>(SplineClass, 
            FVector::ZeroVector, FRotator::ZeroRotator, spawnPara);
        if (SplineActor != nullptr)
        {
            ActorMap.Add(ActorName, SplineActor);
            DrawingSplineActor = SplineActor;
            SplineActor->Spline->ClearSplinePoints(true);
            SplineActor->Width = 50000;
            SplineActor->MeshAxis = ESplineMeshAxis::X;
        }
        
        return SplineActor; 
    }
    return nullptr;
}

void ARigelLevelEditor::ClearRuntimeActors()
{
    for (auto pair : RuntimeMap)
    {
        pair.Value->SetActorHiddenInGame(true);
        pair.Value->Destroy();
    }
    RuntimeMap.Empty();
}

void ARigelLevelEditor::LoadHtmlUrl()
{
    if (WebWidget)
    {
        WebWidget->LoadHTML(HtmlURL);
    }
}

void ARigelLevelEditor::LoadInitServer()
{

}

void ARigelLevelEditor::UseDefaultTerrain()
{
    if (IsValid(World))
    {
        World->SetTilesetSource(ETilesetSource::FromEllipsoid);
    }
}

void ARigelLevelEditor::UpdateTerrainURL(const FString& TerrainUrl)
{
    if (IsValid(World))
    {
        World->SetTilesetSource(ETilesetSource::FromUrl);
        World->SetUrl(TerrainUrl);
        World->RefreshTileset();
    }
}

void ARigelLevelEditor::AddWMSServer(const FString& LayerID, const FString& BaseUrl, const ServiceOption& option)
{
    UCesiumWebMapServiceRasterOverlay* overlay = NewObject<UCesiumWebMapServiceRasterOverlay>(World);
    if (IsValid(World) && IsValid(overlay))
    {
        overlay->BaseUrl = BaseUrl;
        overlay->Layers = option.Layers;
        overlay->MinimumLevel = option.MinLevel;
        overlay->MaximumLevel = option.MaxLevel;
        overlay->MaterialLayerKey = "Overlay1";
        World-> AddInstanceComponent(overlay);
        ServerMap.Add(LayerID, overlay);
    } 
}

void ARigelLevelEditor::AddWMTSServer(const FString& LayerID, const FString& BaseUrl, const ServiceOption& option)
{
    UCesiumWebMapTileServiceRasterOverlay* overlay = NewObject<UCesiumWebMapTileServiceRasterOverlay>(World);
    if (IsValid(World) && IsValid(overlay))
    {
        overlay->BaseUrl = BaseUrl;
        overlay->Layer = option.Layers;
        overlay->MinimumLevel = option.MinLevel;
        overlay->MaximumLevel = option.MaxLevel;
        overlay->MaterialLayerKey = "Overlay1";
        World->AddInstanceComponent(overlay);
        ServerMap.Add(LayerID, overlay);
    }
}

void ARigelLevelEditor::RemoveActor(const FString& ActorID)
{
    AActor* actor = RuntimeMap.FindRef(ActorID);
    if (actor != nullptr)
    {
        actor->SetActorHiddenInGame(true);
        RuntimeMap.Remove(ActorID);
        actor->Destroy();
    }
}
