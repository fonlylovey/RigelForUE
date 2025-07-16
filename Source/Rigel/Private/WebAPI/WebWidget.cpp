#include "WebAPI/WebWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "UObject/ScriptDelegates.h"


UWebWidget::UWebWidget(const FObjectInitializer& ObjectInitializer)
    :Super(ObjectInitializer)
{

}

void UWebWidget::NativeConstruct()
{
    // 创建 CanvasPanel
    CanvasPanel = NewObject<UCanvasPanel>(this);
    if (CanvasPanel)
    {
        // 设置面板属性
        CanvasPanel->SetVisibility(ESlateVisibility::Visible);

        // 添加到根组件
        if (UPanelWidget* RootPanel = Cast<UPanelWidget>(GetRootWidget()))
        {
            RootPanel->AddChild(CanvasPanel);
        }
    }

    WebInterface = NewObject<UWebInterface>(this);
    CanvasPanel->AddChildToCanvas(WebInterface);
    //多语言输入,前端输入栏不能输入中文问题
    WebInterface->EnableIME();
    WebInterface->OnInterfaceEvent.AddDynamic(this, &UWebWidget::OnInterfaceEvent);
   
    APlayerController* Controller = UGameplayStatics::GetPlayerController(GWorld, 0);
    FInputModeGameAndUI inputMode;
    //inputMode.SetWidgetToFocus(WebInterface);
    Controller->SetInputMode(inputMode);
    Controller->SetShowMouseCursor(true);
    Controller->bEnableMouseOverEvents = true;
}

void UWebWidget::OnInterfaceEvent(const FName Name, FJsonLibraryValue Data, FWebInterfaceCallback Callback)
{

}


void UWebWidget::LoadHTML(const FString& URL)
{
    WebInterface->Load(URL);
}

void UWebWidget::LoadFile(const FString& ContentFile)
{
    WebInterface->LoadContent(ContentFile);
}
