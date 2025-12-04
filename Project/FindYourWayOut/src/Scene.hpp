#pragma once

#pragma region include project
#include "ObjectManager.hpp"
#include "Object.hpp"
#pragma endregion

/// <summary>
/// base scene class
/// </summary>
class Scene : public Object
{
public:
	#pragma region destructor
	/// <summary>
	/// destructor
	/// </summary>
	virtual ~Scene();
	#pragma endregion

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