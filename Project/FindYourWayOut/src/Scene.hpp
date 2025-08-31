#pragma once

#pragma region include project
#include "Game.hpp"
#include "ObjectManager.hpp"
#include "Object.hpp"
#pragma endregion

/// <summary>
/// base scene class
/// </summary>
class Scene : public Object
{
public:
	/// <summary>
	/// destructor
	/// </summary>
	virtual ~Scene()
	{
		if (Game::Get().GetLevel())
			// clear all objects including player if menu level
			ObjectManager::Get().ClearLevel(true);
		else
			// clear only level objects if game level
			ObjectManager::Get().ClearLevel(false);
	}

	#pragma region override method
	/// <summary>
	/// initialize scene
	/// </summary>
	virtual void Init() override {}

	/// <summary>
	/// update every frame
	/// </summary>
	/// <param name="_deltaSeconds">time since last frame</param>
	virtual void Update(float _deltaSeconds) override {}

	/// <summary>
	/// render every frame
	/// </summary>
	virtual void Render() override {}
	#pragma endregion
};