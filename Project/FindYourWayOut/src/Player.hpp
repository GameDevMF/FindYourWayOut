#pragma once

#pragma region include project
#include "Object.hpp"
#pragma endregion

#pragma region forward declaration
class Camera;
class HUD;
class Minimap;
#pragma endregion

/// <summary>
/// player class
/// </summary>
class Player : public Object
{
public:
	#pragma region constructor
	/// <summary>
	/// constructor
	/// </summary>
	/// <param name="_location">world location to set</param>
	Player(SVector2 _location) : Object(_location) {}
	#pragma endregion

	#pragma region destructor
	/// <summary>
	/// destructor
	/// </summary>
	~Player();
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
	virtual void Update(float _deltaSeconds) override;

	/// <summary>
	/// render every frame
	/// </summary>
	virtual void Render() override;
	#pragma endregion

	#pragma region inline method
	/// <summary>
	/// set exit location
	/// </summary>
	/// <param name="_location">exit location to set</param>
	inline void SetExitLocation(const SVector2 _location) { m_exitLocation = _location; }

	/// <summary>
	/// get camera reference
	/// </summary>
	/// <returns>camera reference</returns>
	inline Camera& const GetCamera() { return *m_pCamera; }

	/// <summary>
	/// set heads up display reference
	/// </summary>
	/// <returns>heads up display reference</returns>
	inline HUD& const GetHUD() { return *m_pHUD; }
	#pragma endregion

private:
	#pragma region primitive variable
	/// <summary>
	/// time left to finish the level
	/// </summary>
	float m_timeLeft = 0.0f;

	/// <summary>
	/// value to calculate radiant to degree
	/// </summary>
	float m_radiantToDegreeCalculateValue = 0.0f;

	/// <summary>
	/// camera angle in radiant
	/// </summary>
	float m_cameraAngleRadiant = 0.0f;

	/// <summary>
	/// rotation speed in degree per second
	/// </summary>
	float m_rotationSpeed = 30.0f;

	/// <summary>
	/// movement speed in cm per second
	/// </summary>
	float m_movementSpeed = 150.0f;
	#pragma endregion

	#pragma region variable
	/// <summary>
	/// forward vector
	/// </summary>
	SVector2 m_forward;

	/// <summary>
	/// exit location to next level
	/// </summary>
	SVector2 m_exitLocation;
	#pragma endregion

	#pragma region pointer
	/// <summary>
	/// camera reference
	/// </summary>
	Camera* m_pCamera = nullptr;

	/// <summary>
	/// heads up display reference
	/// </summary>
	HUD* m_pHUD = nullptr;

	/// <summary>
	/// mini map reference
	/// </summary>
	Minimap* m_pMinimap = nullptr;
	#pragma endregion
};