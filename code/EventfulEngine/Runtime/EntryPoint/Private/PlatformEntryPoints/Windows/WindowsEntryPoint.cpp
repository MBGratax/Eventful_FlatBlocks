#pragma once
#include "EFEntryPointModuleAPI.h"
#include "Windows/WindowsHeaderWrapper.h"
#include "CoreGlobals.h"
#include "EventfulEngineLoop.h"

namespace EventfulEngine{
	extern int32 GenericMain();

	//extern HINSTANCE g_hInstance;
	EFENTRYPOINT_API int32 WindowsEntryPointStartUp(HINSTANCE hInInstance){
		// TODO: Add Global Profiler marker start
		EF_LOG(CoreLog, info, "Starting up EventfulEngine on Windows!");
		int32 errorLevel = 0;
		// TODO: Once we need a windows handle, assign global ref here
		//g_hInstance = hInInstance;

		g_commandLine.Init(__argc, __argv);

		errorLevel = GenericMain();


		return errorLevel;
	}

	EFENTRYPOINT_API void WindowsEntryPointShutdown(){
		EventfulEngineLoop::AppExit();
	}

}

// ReSharper disable once CppParameterMayBeConst
int WINAPI WinMain(_In_ HINSTANCE hInstance,
                   _In_opt_ HINSTANCE hPrevInstance,
                   _In_ PSTR lpCmdLine,
                   _In_ int nShowCmd){
	const int32 result = EventfulEngine::WindowsEntryPointStartUp(hInstance);
	EventfulEngine::WindowsEntryPointShutdown();
	return result;
}
