#pragma once

#pragma region include project
#include "Enums.hpp"
#include "Scene.hpp"
#pragma endregion

#pragma region include system
#include <memory>
#include <chrono>
#include <string>
#pragma endregion

#pragma region macro
#define NOW std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()).time_since_epoch().count()
#pragma endregion

/// <summary>
/// main game class
/// </summary>
class Game
{
public:
	#pragma region singleton
	/// <summary>
	/// get game instance
	/// </summary>
	/// <returns>game instance</returns>
	static Game& Get()
	{
		static Game instance;
		return instance;
	}
	#pragma endregion

	#pragma region inline method
	/// <summary>
	/// if game is running
	/// </summary>
	/// <returns></returns>
	constexpr bool IsRunning() const noexcept { return m_isRunning; }

	/// <summary>
	/// get current level
	/// </summary>
	/// <returns>current level</returns>
	constexpr int GetLevel() const noexcept { return m_level; }

	/// <summary>
	/// stop game
	/// </summary>
	constexpr void StopGame() noexcept { m_isRunning = false; }

	/// <summary>
	/// get seconds since last frame
	/// </summary>
	/// <returns>seconds since last frame</returns>
	constexpr float GetDeltaSeconds() const noexcept { return m_deltaSeconds; }
	#pragma endregion

	#pragma region method
	/// <summary>
	/// initialize game
	/// </summary>
	void Init();

	/// <summary>
	/// switch to scene
	/// </summary>
	/// <param name="_type">new scene type</param>
	/// <param name="_level">new level</param>
	void SwitchScene(ESCENE_TYPE _type, int _level = 0);

	/// <summary>
	/// switch to error scene
	/// </summary>
	/// <param name="_pText">error text to display</param>
	void ShowErrorScene(const std::string& const _pText);

	/// <summary>
	/// update game every frame
	/// </summary>
	void Update();

	/// <summary>
	/// render game every frame
	/// </summary>
	void Render();
	#pragma endregion

private:
	#pragma region constructor
	/// <summary>
	/// constructor
	/// </summary>
	Game() {};
	#pragma endregion

	#pragma region primitive variable
	/// <summary>
	/// if game is running
	/// </summary>
	bool m_isRunning{ false };

	/// <summary>
	/// if scene needs to change
	/// </summary>
	bool m_isSceneChanging{ false };

	/// <summary>
	/// current level
	/// </summary>
	int m_level{ 0 };

	/// <summary>
	/// seconds since last frame
	/// </summary>
	float m_deltaSeconds{ 0.0f };

	/// <summary>
	/// time of last update
	/// </summary>
	long long m_timeLastUpdate{ NOW };
	#pragma endregion

	#pragma region variable
	/// <summary>
	/// error text
	/// </summary>
	std::string m_errorText{ "" };

	/// <summary>
	/// current scene type
	/// </summary>
	ESCENE_TYPE m_currentSceneType{ ESCENE_TYPE::MENU };
	#pragma endregion

	#pragma region pointer
	/// <summary>
	/// current active scene
	/// </summary>
	std::unique_ptr<Scene> m_pCurrentScene{ nullptr };
	#pragma endregion

	#pragma region method
	/// <summary>
	/// change scene
	/// </summary>
	void ChangeScene();
	#pragma endregion
};