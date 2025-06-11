#pragma once
#include <EFEntryPointModuleAPI.h>
#include <Windows/WindowsHeaderWrapper.h>

#include "EventfulEngineLoop.h"

namespace EventfulEngine{
	extern int32 GenericMain(const TCHAR* CmdLine);

	extern HINSTANCE g_hInstance;
	EFENTRYPOINT_API int32 WindowsEntryPointStartUp(HINSTANCE hInInstance, HINSTANCE hPrevInstance, char*,
	                                                int32 nCmdShow,
	                                                const TCHAR* CmdLine){
		// TODO: Add Global Profiler marker start
		// TODO: Log startup info

		int32 ErrorLevel = 0;
		g_hInstance = hInInstance;

		// TODO: Once we have commandline stuff, handle it here


		ErrorLevel = GenericMain(CmdLine);


		return ErrorLevel;
	}

	EFENTRYPOINT_API void WindowsEntryPointShutdown(){
		EventfulEngineLoop::AppExit();
	}

	int32 WINAPI WinMain(HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, PSTR pCmdLine, int32 nCmdShow){
		int32 Result = WindowsEntryPointStartUp(hInstance, hPrevInstance, pCmdLine, nCmdShow, nullptr);
		WindowsEntryPointShutdown();
		return Result;
	}
}
