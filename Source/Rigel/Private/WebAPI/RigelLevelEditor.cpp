#include "WebAPI/RigelLevelEditor.h"

#include "EngineUtils.h"

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
    AActor* actor = nullptr;
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
