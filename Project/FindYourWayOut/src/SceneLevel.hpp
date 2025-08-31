#pragma once

#pragma region include project
#include "Scene.hpp"
#pragma endregion

/// <summary>
/// main level scene class
/// </summary>
class SceneLevel : public Scene
{
public:
	#pragma region override method
	/// <summary>
	/// initialize scene
	/// </summary>
	virtual void Init() override;

	/// <summary>
	/// update every frame
	/// </summary>
	/// <param name="_deltaSeconds">time since last frame</param>
	virtual void Update(float _deltaSeconds) override;

	/// <summary>
	/// render every frame
	/// </summary>
	virtual void Render() override;
	#pragma endregion
};