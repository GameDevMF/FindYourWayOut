#pragma region include project
#include "Scene.hpp"
#include "Game.hpp"
#pragma endregion

Scene::~Scene()
{
	if (Game::Get().GetLevel())
		// clear all objects including player if menu level
		ObjectManager::Get().ClearLevel(true);
	else
		// clear only level objects if game level
		ObjectManager::Get().ClearLevel(false);
}