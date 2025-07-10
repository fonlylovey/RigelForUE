#include "RigelEditorModule.h"
#include "Modules/ModuleManager.h"
#include "LevelEditor.h"
#include "Framework/MultiBox/MultiBoxExtender.h"
#include "SceneOutlinerMenuContext.h"
#include "SSceneOutliner.h"
#include "Widgets/Views/SListView.h"
#include "Internationalization/Internationalization.h"


IMPLEMENT_MODULE(FRigelEditor, RigelEditor);

#define LOCTEXT_NAMESPACE "RigelEditor"

static const FName DefaultContextMenuName("SceneOutliner.DefaultContextMenu");

void FRigelEditor::StartupModule()
{
    if (!IsRunningCommandlet())
    {
        FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked< FLevelEditorModule >("LevelEditor");

        TArray<FLevelEditorModule::FLevelViewportMenuExtender_SelectedActors >& menuExtenderDelegates = LevelEditorModule.GetAllLevelViewportContextMenuExtenders();



        menuExtenderDelegates.Add(FLevelEditorModule::FLevelViewportMenuExtender_SelectedActors::CreateRaw(this, &FRigelEditor::OnExtendEarthEditorSectionGruop));
    }


}

void FRigelEditor::ShutdownModule()
{/*
    if (FModuleManager::Get().IsModuleLoaded("SceneOutliner"))
    {
        FSceneOutlinerModule& SceneOutlinerModule = FModuleManager::GetModuleChecked<FSceneOutlinerModule>("SceneOutliner");
        SceneOutlinerModule.GetGlobalMenuExtensibilityManager()->RemoveExtender(MenuExtender);
    }*/
}

TSharedRef<FExtender> FRigelEditor::OnExtendEarthEditorSectionGruop(const TSharedRef<FUICommandList> CommandList, TArray<AActor*> SelectedActors)
{
    TSharedRef<FExtender> Extender = MakeShared<FExtender>();
    Extender->AddMenuExtension("Rigel", EExtensionHook::After, nullptr, FMenuExtensionDelegate::CreateLambda(
        [SelectedActors](FMenuBuilder& MenuBuilder)
        {

            //菜单栏中的分组，无法交互，只用于分组
            MenuBuilder.BeginSection("Rigel Tool", LOCTEXT("RigelMenuSection", "Rigel Gis"));
            //NSLOCTEXT主要用于多语言支持(命名空间，文本唯一标，示本地化文本)
            MenuBuilder.AddSubMenu(
                NSLOCTEXT(LOCTEXT_NAMESPACE, "ActorMenu_RigelTools", "坐标转换"),
                NSLOCTEXT(LOCTEXT_NAMESPACE, "ActorMenu_RigelTools_ToolTip", "坐标转"),
                FNewMenuDelegate::CreateStatic(&FRigelEditor::OnAddEarthEditorSubMenu, SelectedActors),
                false,
                FSlateIcon());
            MenuBuilder.EndSection();

        }));
    return Extender;
}

void FRigelEditor::OnAddEarthEditorSubMenu(FMenuBuilder& MenuBuilder, TArray<AActor*> SelectedActors)
{

}

#undef LOCTEXT_NAMESPACE