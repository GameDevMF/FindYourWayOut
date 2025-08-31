#pragma once

#pragma region include system
#include <windows.h>
#pragma endregion

#pragma region include project
#include "Scene.hpp"
#pragma endregion

/// <summary>
/// game win scene class
/// </summary>
class SceneGameWin : public Scene
{
	#pragma region destructor
	/// <summary>
	/// destructor
	/// </summary>
	~SceneGameWin();
	#pragma endregion

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

private:
	#pragma region variable
	/// <summary>
	/// start text location on screen
	/// </summary>
	SVector2 m_startTextLocation = SVector2();
	#pragma endregion

	#pragma region pointer
	/// <summary>
	/// welcome text reference
	/// </summary>
	CHAR_INFO* m_pGameWin = nullptr;

	/// <summary>
	/// start text reference
	/// </summary>
	CHAR_INFO* m_pMenu = nullptr;
	#pragma endregion
};