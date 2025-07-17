#include "WebAPI/WebWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "UObject/ScriptDelegates.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanelSlot.h"
#include "Internationalization/Internationalization.h"
#include "Blueprint/WidgetTree.h"
#include "WebAPI/RigelAPISubsystem.h"

#define LOCTEXT_NAMESPACE "WebWidget"

UWebWidget::UWebWidget(const FObjectInitializer& ObjectInitializer)
    :Super(ObjectInitializer)
{

}

void UWebWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (WebInterface != nullptr)
    {
        //多语言输入,前端输入栏不能输入中文问题
        WebInterface->EnableIME();
        WebInterface->OnInterfaceEvent.AddDynamic(this, &UWebWidget::OnInterfaceEvent);
    }
    
   
    APlayerController* Controller = UGameplayStatics::GetPlayerController(GWorld, 0);
    FInputModeGameAndUI inputMode;
    //inputMode.SetWidgetToFocus(WebInterface);
    Controller->SetInputMode(inputMode);
    Controller->SetShowMouseCursor(true);
    Controller->bEnableMouseOverEvents = true;
}

void UWebWidget::OnInterfaceEvent(const FName Name, FJsonLibraryValue Data, FWebInterfaceCallback Callback)
{
    URigelAPISubsystem* subsystem = GetWorld()->GetGameInstance()->GetSubsystem<URigelAPISubsystem>();
    FJsonLibraryObject rootObj = FJsonLibraryObject::Parse(Data.GetString());
    subsystem->Invoke(Name.ToString(), rootObj.GetObject(TEXT("Data")));
}


void UWebWidget::LoadHTML(const FString& URL)
{
    WebInterface->LoadURL(URL);
}

void UWebWidget::LoadFile(const FString& ContentFile)
{
    WebInterface->LoadFile(ContentFile, EWebInterfaceDirectory::Content);
}
