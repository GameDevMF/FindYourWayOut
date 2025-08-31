#pragma region include project
#include "ObjectManager.hpp"
#include "HUD.hpp"
#include "Raytracer.hpp"
#include "Game.hpp"
#include "SceneLevel.hpp"
#pragma endregion

void SceneLevel::Init()
{
	ObjectManager::Get().LoadLevel();
}

void SceneLevel::Update(float _deltaSeconds)
{
	ObjectManager::Get().Update(_deltaSeconds);
}

void SceneLevel::Render()
{
	Raytracer::Get().Render();
	ObjectManager::Get().Render();
}