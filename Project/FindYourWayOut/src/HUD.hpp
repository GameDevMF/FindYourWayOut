#pragma once

#pragma region system include
#include <windows.h>
#pragma endregion

#pragma region include project
#include "Object.hpp"
#pragma endregion

/// <summary>
/// game heads up display class
/// </summary>
class HUD : public Object
{
public:
	#pragma region constructor
	/// <summary>
	/// constructor
	/// </summary>
	HUD() : Object({ 0.0f, 0.0f }) {}
	#pragma endregion

	#pragma region destructor
	/// <summary>
	/// destructor
	/// </summary>
	~HUD();
	#pragma endregion

	#pragma region virtual method
	/// <summary>
	/// initialize object
	/// </summary>
	virtual void Init() override;

	/// <summary>
	/// update every frame
	/// </summary>
	/// <param name="_deltaSeconds">time since last frame</param>
	virtual void Update(float _deltaSeconds) override {}

	/// <summary>
	/// render every frame
	/// </summary>
	virtual void Render() override;
	#pragma endregion

	#pragma region method
	/// <summary>
	/// set level
	/// </summary>
	void UpdateLevel();

	/// <summary>
	/// update time left
	/// </summary>
	/// <param name="_time">current time left</param>
	void UpdateTimeLeft(const int _time);
	#pragma endregion

private:
	#pragma region pointer
	/// <summary>
	/// mini map char info
	/// </summary>
	CHAR_INFO* m_pCharInfo = nullptr;
	#pragma endregion
};