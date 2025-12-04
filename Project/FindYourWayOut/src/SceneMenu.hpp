#pragma once

#pragma region include system
#include <windows.h>
#pragma endregion

#pragma region include project
#include "Scene.hpp"
#pragma endregion

/// <summary>
/// menu scene class
/// </summary>
class SceneMenu : public Scene
{
public:
	#pragma region destructor
	/// <summary>
	/// destructor
	/// </summary>
	~SceneMenu();
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
	/// start text position on screen
	/// </summary>
	SVector2 m_startTextPosition{ 0.0f, 0.0f };
	#pragma endregion

	#pragma region pointer
	/// <summary>
	/// welcome text reference
	/// </summary>
	CHAR_INFO* m_pWelcome{ nullptr };

	/// <summary>
	/// start text reference
	/// </summary>
	CHAR_INFO* m_pStart{ nullptr };
	#pragma endregion
};