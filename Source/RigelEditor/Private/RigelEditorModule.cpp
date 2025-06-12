#include "RigelEditorModule.h"
#include "Modules/ModuleManager.h"
#include "DetailsViewpointCustomization.h"

IMPLEMENT_MODULE(FRigelEditor, RigelEditor);

void FRigelEditor::StartupModule()
{
    FPropertyEditorModule& EditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
    EditorModule.RegisterCustomClassLayout(FName(TEXT("Viewpoint")), 
        FOnGetDetailCustomizationInstance::CreateStatic(&FDetailsViewpointCustomization::MakeInstance));
}

void FRigelEditor::ShutdownModule()
{
    FPropertyEditorModule& EditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
    EditorModule.UnregisterCustomClassLayout(FName(TEXT("Viewpoint")));
}