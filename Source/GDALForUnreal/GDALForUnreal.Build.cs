// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;

namespace UnrealBuildTool.Rules
{
	public class GDALForUnreal : ModuleRules
	{
		public GDALForUnreal(ReadOnlyTargetRules Target) : base(Target)
		{

            string GDALPath = ModuleDirectory;

			if (Target.Platform == UnrealTargetPlatform.Win64)
			{
				string IncludePath = Path.Combine(GDALPath, "include");
				string LibPath = Path.Combine(GDALPath, "lib");
				string BinPath = Path.Combine(GDALPath, "bin");

				PublicIncludePaths.Add(IncludePath);
                PublicIncludePaths.Add(Path.Combine(IncludePath, "proj"));
                string PlatformPath = Target.Platform.ToString();

				if (Target.Configuration == UnrealTargetConfiguration.Debug)
				{
					PublicAdditionalLibraries.Add(Path.Combine(LibPath, PlatformPath, "gdald.lib"));
					RuntimeDependencies.Add(Path.Combine(BinPath, PlatformPath, "gdald.dll"));
					PublicDelayLoadDLLs.Add("gdald.dll");
				}
				else
				{
					PublicAdditionalLibraries.Add(Path.Combine(LibPath, PlatformPath, "gdal.lib"));
					RuntimeDependencies.Add(Path.Combine(BinPath, PlatformPath, "gdal.dll"));
					PublicDelayLoadDLLs.Add("gdal.dll");
				}

                RuntimeDependencies.Add(Path.Combine(GDALPath, "share", "proj", "proj.db"));
                RuntimeDependencies.Add(Path.Combine(GDALPath, "share", "proj", "proj.ini"));

                PublicDefinitions.Add("WIN32");
                //PublicDefinitions.Add("__clang_major__=0");
                CppStandard = CppStandardVersion.Cpp17;

            }
			else if (Target.Platform == UnrealTargetPlatform.Linux)
			{
                string GDALLinuxPath = Path.Combine(GDALPath, "Linux-x86_64");

                string IncludePath = Path.Combine(GDALPath, "Linux-x86_64", "include");
				string LibraryPath = Path.Combine(GDALPath, "Linux-x86_64", "lib");
				PublicIncludePaths.Add(IncludePath);
                PublicIncludePaths.Add(Path.Combine(IncludePath, "proj"));

                PrivateRuntimeLibraryPaths.Add(LibraryPath);

                PublicAdditionalLibraries.Add(Path.Combine(LibraryPath, "libcurl.so"));
                PublicAdditionalLibraries.Add(Path.Combine(LibraryPath, "libproj.so"));
                PublicAdditionalLibraries.Add(Path.Combine(LibraryPath, "libgdal.so"));


                RuntimeDependencies.Add(Path.Combine(LibraryPath, "libcurl.so"));
                RuntimeDependencies.Add(Path.Combine(LibraryPath, "libproj.so"));
                RuntimeDependencies.Add(Path.Combine(LibraryPath, "libgdal.so"));

                RuntimeDependencies.Add(Path.Combine(GDALLinuxPath, "share", "proj", "proj.db"));
                RuntimeDependencies.Add(Path.Combine(GDALLinuxPath, "share", "proj", "proj.ini"));
            }
			else
			{
				System.Console.WriteLine("GdalForUnreal did not load correctly");
			}

			PublicIncludePaths.AddRange(
				new string[] {
					// ... add public include paths required here ...
				}
				);


			PrivateIncludePaths.AddRange(
				new string[] {
					// ... add other private include paths required here ...
				}
				);


			PublicDependencyModuleNames.AddRange(
				new string[]
				{
					"Core",
					"CoreUObject",
					"Engine",
					"Projects"
					// ... add other public dependencies that you statically link with here ...
				}
				);


			PrivateDependencyModuleNames.AddRange(
				new string[]
				{
					// ... add private dependencies that you statically link with here ...	
				}
				);


			DynamicallyLoadedModuleNames.AddRange(
				new string[]
				{
					// ... add any modules that your module loads dynamically here ...
				}
				);
		}
	}
}