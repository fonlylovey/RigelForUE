#include "WebAPI/RigelLevelEditor.h"

#include "EngineUtils.h"
#undef OPAQUE
#include "Cesium3DTileset.h"
#include "WebAPI/WebWidget.h"

// Sets default values
ARigelLevelEditor::ARigelLevelEditor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

AActor* ARigelLevelEditor::GetActor(const FString& Name)
{
    AActor* actor = ActorMap.FindRef(Name);
    return  actor;
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
            if (theActor->GetActorLabel().Equals(Name))
            {
                actor = theActor;
                break;
            }
        }
	}
    return actor;
}

void ARigelLevelEditor::SetLayerVisibility(const FString& LayerID, bool isVisible)
{
    AActor* layer = LayerMap.FindRef(LayerID);
    if (layer != nullptr)
    {
        layer->SetActorHiddenInGame(!isVisible);
    }
}

void ARigelLevelEditor::Add3DTiles(const FString& LayerID, const FString& URL)
{
    if (GetWorld() != nullptr)
    {
        FActorSpawnParameters spawnPara;
        spawnPara.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        ACesium3DTileset* tileset = GetWorld()->SpawnActor<ACesium3DTileset>(
            FVector::ZeroVector, FRotator::ZeroRotator, spawnPara);
        tileset->SetTilesetSource(ETilesetSource::FromUrl);
        tileset->SetUrl(URL);
        LayerMap.Add(LayerID, tileset);
    }
}

void ARigelLevelEditor::RemoveLayer(const FString& LayerID)
{
    AActor* actor = LayerMap.FindRef(LayerID);
    if (actor != nullptr)
    {
        actor->SetActorHiddenInGame(true);
        LayerMap.Remove(LayerID);
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
    if (WebWidget != nullptr)
    {
        WebWidget->SendMessage(Function, Data);
    }
}

void ARigelLevelEditor::SetWebWIdget(UWebWidget* widget)
{
    WebWidget = widget;
}

// Called when the game starts or when spawned
void ARigelLevelEditor::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ARigelLevelEditor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
