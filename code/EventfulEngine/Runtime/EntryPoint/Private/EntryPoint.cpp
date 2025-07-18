#pragma once

#include "EFEntryPointModuleAPI.h"
#include "../../Core/Public/CoreMinimal.h"
#include "EventfulEngineLoop.h"
#include "CoreGlobals.h"

#include "GenericPlatform/GenericPlatformTime.h"

namespace EventfulEngine{
	EventfulEngineLoop g_engineLoop;

#ifndef WITH_EDITOR
#define WITH_EDITOR 0
#endif

	/**
	 * PreInits the engine loop
	 */
	int32 BeforeEngineInit(){
		const int32 errorLevel = g_engineLoop.BeforeEngineInit();

		return errorLevel;
	}

	/**
	 * Inits the engine loop
	 */
	int32 EngineInit(){
		const int32 errorLevel = g_engineLoop.Init();

		return errorLevel;
	}

	/**
	 * Ticks the engine loop
	 */
	EFENTRYPOINT_API void EngineTick(){
		g_engineLoop.Tick();
	}

	/**
	 * Shuts down the engine
	 */
	EFENTRYPOINT_API void EngineExit(){
		g_engineLoop.Exit();
	}

	/*!
	 * @brief Generic main entry point that is invoked by the platform-specific entry point after setting up platform boilerplate
	 * @return The usual 0 for success and >0 for error int
	 */
	int32 GenericMain(){
		// In-method struct that calls engine exit whenever the scope is left
		struct EngineLoopCleanupGuard{

			NOMOVEORCOPY(EngineLoopCleanupGuard)

			EngineLoopCleanupGuard() = default;

			~EngineLoopCleanupGuard(){
				EngineExit();
			}
		};

		EngineLoopCleanupGuard cleanupGuard;

		// PreInit code (use cmd stuff, load modules, etc)
		int32 errorLevel = BeforeEngineInit();

		// exit if PreInit failed, or engine was immediately requested to stop.
		if (errorLevel != 0 || IsEngineExitRequested()){
			return errorLevel;
		}

		{
#if WITH_EDITOR
			if (g_isEditor){
				ErrorLevel = EditorInit(GEngineLoop);
			}
			else
#endif
			{
				errorLevel = EngineInit();
			}
		}
		EFPlatformTime::Init();
		double engineInitializationTime = EFPlatformTime::ToSeconds(
			EFPlatformTime::lastTime - EFPlatformTime::appStartTime);

		// If we are embedded i.e., in the editor or another custom application, let them handle the engine ticking
		if (!g_gameEditorOverrides.bIsEmbedded){
			while (!IsEngineExitRequested()){
				EngineTick();
			}
		}

#if WITH_EDITOR
		if (g_isEditor){
			EditorExit();
		}
#endif
		return errorLevel;
	}
}
