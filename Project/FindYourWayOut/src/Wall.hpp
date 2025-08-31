#pragma once

#pragma region inlcude project
#include "Object.hpp"
#pragma endregion

/// <summary>
/// class that represents a square wall tile
/// </summary>
class Wall : public Object
{
public:
	#pragma region constructor
	/// <summary>
	/// constructor
	/// </summary>
	/// <param name="_location">world location to set</param>
	Wall(SVector2 _location) : Object(_location) {}
	#pragma endregion

	#pragma region override method
	/// <summary>
	/// initialize object
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