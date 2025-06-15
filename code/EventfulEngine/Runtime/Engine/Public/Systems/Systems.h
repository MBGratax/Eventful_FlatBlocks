#pragma once

/* TODO: The Systems header holds includes for all systems related functionality. Includes:
 * - EFSystem: Class that describes an engine level system. These systems can hold state and hook into a Tick hook or
 *   create a custom one. Holds a collection of Subsystems it can tick.
 * - EFSubSystem: Class that can be held and ticked by an EFSystem.
 * - EFSystemManager: Singleton that handles system and subsystem registration and ticking.
 *   Whenever the app ticks the engine, the engine ticks the manager, which then propagates ticks to the systems.
 *   Usually users are recommended to add subsystems to the EFGameSystem though.
 * - EFGameSystem: EFEngineSystem that handles default engine tick processes
 * - Outside of this module there are a couple more standard systems, like the RendererSystem and Physics System.
 */
