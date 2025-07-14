#include "GDALForUnreal.h"

#include "Misc/Paths.h"
#include "Interfaces/IPluginManager.h"
#include "Misc/MessageDialog.h"
#include "GISFunctionLibrary.h"
#include <gdal_priv.h>  
#include <gdal.h>  
#include <ogrsf_frmts.h>
#include "cpl_conv.h"

#define LOCTEXT_NAMESPACE "FGDALForUnrealModule"



void* FGDALForUnrealModule::GDALLibraryHandle = nullptr;


void FGDALForUnrealModule::StartupModule()
{
#if PLATFORM_WINDOWS
	FString BaseDir = IPluginManager::Get().FindPlugin("Rigel")->GetBaseDir();
	FString LibraryPath;


#if UE_BUILD_DEBUG
	LibraryPath = FPaths::Combine(*BaseDir, TEXT("Source/GDALForUnreal/bin/Win64/gdald.dll"));
#else
	LibraryPath = FPaths::Combine(*BaseDir, TEXT("Source/GDALForUnreal/bin/Win64/gdal.dll"));
#endif


	GDALLibraryHandle = !LibraryPath.IsEmpty() ? FPlatformProcess::GetDllHandle(*LibraryPath) : nullptr;
	if (GDALLibraryHandle)
	{
		UE_LOG(LogTemp, Log, TEXT("============= GDAL  OnStartup  ============="));
		UE_LOG(LogTemp, Log, TEXT("==== GDALAllRegister ===="));

		GDALAllRegister();
		CPLSetConfigOption("GDAL_CACHEMAX", "1000");
		CPLSetConfigOption("OGR_PG_CURSOR_PAGE", "10000");

		CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");//支持中文路径
		CPLSetConfigOption("SHAPE_ENCODING", "");//防止生成的中文字段属性出现乱码

		//set the path of the proj db 
		FString ProjDBPath = FPaths::Combine(*BaseDir, TEXT("Source/GDALForUnreal/share/proj"));
		ANSICHAR* ansiPath = TCHAR_TO_ANSI(*ProjDBPath);
		const char* proj_path[] = {ansiPath, nullptr};
		OSRSetPROJSearchPaths(proj_path);

	}
	else
	{
		FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("ThirdPartyLibraryError", "Failed to load GDALForUnreal."));
	}

#elif PLATFORM_LINUX

	FString BaseDir = IPluginManager::Get().FindPlugin("GDALForUnreal")->GetBaseDir();
	UE_LOG(LogTemp, Log, TEXT("============= GDAL  OnStartup  ============="));
	UE_LOG(LogTemp, Log, TEXT("==== GDALAllRegister ===="));

	GDALAllRegister();
	CPLSetConfigOption("GDAL_CACHEMAX", "1000");
	CPLSetConfigOption("OGR_PG_CURSOR_PAGE", "10000");

	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");//支持中文路径
	CPLSetConfigOption("SHAPE_ENCODING", "");//防止生成的中文字段属性出现乱码

	//set the path of the proj db 
	FString ProjDBPath = FPaths::Combine(*BaseDir, TEXT("Source/GDALForUnreal/Linux-x86_64/share/proj"));
	ANSICHAR* ansiPath = TCHAR_TO_ANSI(*ProjDBPath);
	const char* proj_path[] = { ansiPath, nullptr };
	OSRSetPROJSearchPaths(proj_path);

#endif
}

void FGDALForUnrealModule::ShutdownModule()
{
#if PLATFORM_WINDOWS
	FPlatformProcess::FreeDllHandle(GDALLibraryHandle);
	GDALLibraryHandle = nullptr;
#endif // PLATFORM_WINDOWS
}


bool FGDALForUnrealModule::IsSupportGDAL()
{
#if PLATFORM_WINDOWS
	return GDALLibraryHandle == nullptr ? false : true;
#elif PLATFORM_LINUX
	return true;
#endif
}

#undef LOCTEXT_NAMESPACE



IMPLEMENT_MODULE(FGDALForUnrealModule, GDALForUnreal)
