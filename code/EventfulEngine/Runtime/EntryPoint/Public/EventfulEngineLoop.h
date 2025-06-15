#pragma once

#include "../../Core/Public/CoreMinimal.h"

#ifndef WITH_ENGINE
#define WITH_ENGINE 1
#endif

#if WITH_ENGINE
#include "../../Engine/Public/Engine/EventfulEngine.h"
#endif

/*!When the entry point is hit, it creates an EventfulEngineLoop. This loop initializes everything it needs until it
 * passes its duty to other subsystems. It handles loading modules in the correct sequence, and initializing the Editor and Game Instance if needed.
 * @brief The standard loop for any Eventful Engine Application (Editor, Game, App).
 * Extends the engine loop if built with WITH_ENGINE defined
 */
namespace EventfulEngine{
	class EventfulEngineLoop final
#if WITH_ENGINE
		: public IEngineLoop
#endif
	{
	public:
		EventfulEngineLoop() = default;

		~EventfulEngineLoop() override = default;

		/*!
	 * @brief The Pre-initialization of the main loop. Generates a commandline from the main() arguments.
	 * @param ArgC The number of strings in ArgV.
	 * @param ArgV The command line parameters where the first element (ArgV[0]) is expected to be the name of the executable.
	 * @param AdditionalCommandline An optional string to append to the cli after processing ArgV.
	 * @return Returns the error level as usual, 0 is success, and >0 means there were errors.
	 */
		int32 PreInitProcessCLI(int32 ArgC, const EFString ArgV[], const EFString* AdditionalCommandline = nullptr);

		/*!
	 * @brief Runs the actual Preinitialization steps.
	 * @param CmdLine The previously generated commandline.
	 * @return Returns the error level as usual, 0 is success, and >0 means there were errors.
	 */
		int32 BeforeEngineInit(const EFString& CmdLine);

		/** First part of PreInit. */
		int32 PreInitPreStartupScreen(const EFString& CmdLine);

		/** Second part of PreInit. */
		int32 PreInitPostStartupScreen(const EFString& CmdLine);

		/** Load all modules needed before Init. */
		void LoadPreInitModules();

		/** Load core modules. */
		bool LoadCoreModules();

		/** Allow override of the project module loaded from project file */
		void OverrideProjectModule(const EFString& InOriginalProjectModuleName,
		                           const EFString& InReplacementProjectModuleName);

		/** Clean up PreInit context. */
		void CleanupPreInitContext();

#if WITH_ENGINE
		/** Load all core modules needed at startup time. */
		bool LoadStartupCoreModules();

		/** Load all modules needed at startup time. */
		bool LoadStartupModules();

		/**
		 * Initialize the main loop (the rest of the initialization).
		 *
		 * @return The error level; 0 if successful, > 0 if there were errors.
		 */
		int32 Init() override;

		/** Initialize the timing options from the command line. */
		void InitTime();

		/** Performs shut down. */
		void Exit();

		/** Whether the engine should operate in an idle mode that uses no CPU or GPU time. */
		[[nodiscard]] bool ShouldUseIdleMode() const;

		/** Advances the main loop. */
		void Tick() override;

		/** Removes references to any objects pending cleanup by deleting them. */
		void ClearPendingCleanupObjects() override;

#endif // WITH_ENGINE

		/** RHI post-init initialization */
		static void PostInitRHI();

		/** Initializes the application. */
		static bool AppInit();

		/**
	 * Prepares the application for shutdown.
	 *
	 * This function is called from within guarded exit code, only during non-error exits.
	 */
		static void AppPreExit();

		/**
	 * Shuts down the application.
	 *
	 * This function called outside guarded exit code, during all exits (including error exits).
	 */
		static void AppExit();
	};

	// Declare global engine loop
	extern EventfulEngineLoop g_engineLoop;

}
