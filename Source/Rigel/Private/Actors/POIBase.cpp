// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/POIBase.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/Character.h"

#if WITH_EDITOR
#include "Editor/UnrealEd/Public/EditorViewportClient.h"
#endif

// Sets default values
APOIBase::APOIBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
    WidgetComponent->SetupAttachment(RootComponent);
}

void APOIBase::OnClickedActorEvent_Implementation(AActor* TouchedActor, FKey ButtonPressed)
{

}

void APOIBase::OnClickedComonetEvent_Implementation(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed)
{

}

// Called when the game starts or when spawned
void APOIBase::BeginPlay()
{
	Super::BeginPlay();
    WidgetComponent->OnClicked.AddDynamic(this, &APOIBase::OnClickedComonetEvent);
    OnClicked.AddDynamic(this, &APOIBase::OnClickedActorEvent);
}

void APOIBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
}

void APOIBase::BeginDestroy()
{
    Super::BeginDestroy();
}

#if WITH_EDITOR
void APOIBase::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);
    if (!PropertyChangedEvent.Property)
    {
        return;
    }

    FName propertyName = PropertyChangedEvent.Property->GetFName();

    if (propertyName == GET_MEMBER_NAME_CHECKED(APOIBase, WidgetComponent))
    {
        createWidget();
    }
   
    WidgetComponent->MarkRenderStateDirty();
    WidgetComponent->Modify();
}
#endif

void APOIBase::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);
    createWidget();
}

bool APOIBase::ShouldTickIfViewportsOnly() const
{
    return IsEditorTick;
}

// Called every frame
void APOIBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    
    if (pPlayerController == nullptr)
    {
        pPlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    }
   
    FVector cameraLocation;
    FRotator cameraRotation;
    if (GetWorld()->HasBegunPlay())
    {
        cameraLocation = pPlayerController->PlayerCameraManager->GetCameraLocation();
        cameraRotation = pPlayerController->GetControlRotation();
    }
    else
    {
#if WITH_EDITOR
        //FEditorViewportClient* client = (FEditorViewportClient*)GEditor->GetActiveViewport()->GetClient();
        //cameraLocation = client->GetViewLocation();
        //cameraRotation = client->GetViewRotation();
#endif
    }

    if (WidgetComponent != nullptr)
    {
        //朝向屏幕
        FVector location = this->GetActorLocation();
        
        FRotator lookAt = UKismetMathLibrary::FindLookAtRotation(location, cameraLocation);
        WidgetComponent->SetWorldRotation(FRotator(0, lookAt.Yaw, 0));
        //自动缩放
       
        double dis = FVector::Distance(location, cameraLocation);
        int scale = (int)dis / ScaleFactor * 0.01;
        if (AtMaxHide)
        {
            bool hideActor = scale > MaxScale ? true : false;
            this->SetActorHiddenInGame(hideActor);
        }
        scale = FMath::Clamp(scale, 1, MaxScale);
        RootComponent->SetWorldScale3D(FVector(scale, scale, scale));
        WidgetComponent->SetRelativeLocation(InitPostion);
        
    }
}

void APOIBase::createWidget()
{
    if (FSlateApplication::IsInitialized())
    {
        if (UWorld* World = GetWorld())
        {
           
        }
    }
}

void APOIBase::updateGeo(const FTransform& Transform)
{
    
}

