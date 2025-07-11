#pragma once


#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"

class GDALFORUNREAL_API FGDALForUnrealModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	static bool IsSupportGDAL();
private:
	static void* GDALLibraryHandle;
};
