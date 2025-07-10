#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"

class FToolBarBuilder;
class FMenuBuilder;

class FRigelEditor : public FDefaultModuleImpl
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

    //
    TSharedRef<FExtender> OnExtendEarthEditorSectionGruop(
        const TSharedRef<FUICommandList> CommandList, TArray<AActor*> SelectedActors);

    static void OnAddEarthEditorSubMenu(FMenuBuilder& MenuBuilder, TArray<AActor*> SelectedActors);
};
