#pragma once
#include "EFEntryPointModuleAPI.h"
#include "Windows/WindowsHeaderWrapper.h"
#include "CoreGlobals.h"

#include "EventfulEngineLoop.h"

namespace EventfulEngine{
	extern int32 GenericMain(const EFChar* CmdLine);

	//extern HINSTANCE g_hInstance;
	EFENTRYPOINT_API int32 WindowsEntryPointStartUp(HINSTANCE hInInstance, HINSTANCE hPrevInstance, char*,
	                                                int32 nCmdShow,
	                                                const EFChar* CmdLine){
		// TODO: Add Global Profiler marker start
		EF_LOG(CoreLog, info, "Starting up EventfulEngine on Windows!");
		int32 ErrorLevel = 0;
		//g_hInstance = hInInstance;

		// TODO: Once we have commandline stuff, handle it here


		ErrorLevel = GenericMain(CmdLine);


		return ErrorLevel;
	}

	EFENTRYPOINT_API void WindowsEntryPointShutdown(){
		EventfulEngineLoop::AppExit();
	}

}

int32 WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ char* pCmdLine,
                     _In_ int32 nCmdShow){
	int32 Result = EventfulEngine::WindowsEntryPointStartUp(hInstance, hPrevInstance, pCmdLine, nCmdShow, nullptr);
	EventfulEngine::WindowsEntryPointShutdown();
	return Result;
}
