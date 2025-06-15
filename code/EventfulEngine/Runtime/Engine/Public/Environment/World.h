#pragma once

/* TODO: Collection of Classes and structs that describe the game World.
 * - World: Class that describes the world of a game. Holds WorldData, LevelCollection and connects to LevelStreaming API.
 * - WorldData: Struct that holds engine level data for the world. Needed to query world state mostly in the editor.
 *   Knows the type of world (Editor [i.e., unsimulated edit mode world], Game [i.e., runtime game mode world], PIE [Play in Editor])
 * - LevelURL: Struct that holds URL like data for levels
 * - Level: Class that holds GameObjects in a scenetree. Can be serialized and loaded into a world.
 * - IProxyWorld interface with methods to help convert and sync gameobjects to proxies for "Parrallel worlds" (e.g. RenderScene and PhysicsWorld)
 */
class World{

};
