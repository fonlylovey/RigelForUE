#include "WebAPI/RigelPOIWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "UObject/ScriptDelegates.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanelSlot.h"
#include "Internationalization/Internationalization.h"
#include "Blueprint/WidgetTree.h"
#include "WebAPI/RigelAPISubsystem.h"
#include "WebAPI/RigelLevelEditor.h"

#define LOCTEXT_NAMESPACE "URigelPOIWidget"

URigelPOIWidget::URigelPOIWidget(const FObjectInitializer& ObjectInitializer)
    :Super(ObjectInitializer)
{

}

void URigelPOIWidget::OnClickedUMG()
{
    if (OnUMGClicked.IsBound())
    {
        OnUMGClicked.Execute();
    }
}

void URigelPOIWidget::NativeConstruct()
{
    Super::NativeConstruct();

   
}
