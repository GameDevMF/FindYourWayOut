#pragma once

#pragma region include project
#include "Vector2.hpp"
#include "Object.hpp"
#pragma endregion

/// <summary>
/// main camera class
/// </summary>
class Camera : public Object
{
public:
	#pragma region constructor
	/// <summary>
	/// constructor
	/// </summary>
	/// <param name="_location">world location to set</param>
	Camera(SVector2 _location) : Object(_location) {}
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

	#pragma region inline method
	/// <summary>
	/// get field of view
	/// </summary>
	/// <returns>field of view</returns>
	inline SVector2 GetFov() const { return m_fov; }

	/// <summary>
	/// set field of view
	/// </summary>
	/// <param name="_fov">field of view to set</param>
	inline void SetFov(SVector2 _fov) { m_fov = _fov; }

	/// <summary>
	/// get angle of camera
	/// </summary>
	/// <returns>angle of camera</returns>
	constexpr float GetAngle() const noexcept { return m_angle; }

	/// <summary>
	/// set angle of camera
	/// </summary>
	/// <param name="_fov">angle to set</param>
	constexpr void SetAngle(float _angle) noexcept { m_angle = _angle < 0.0f ? _angle + 360.0f : _angle; }

	/// <summary>
	/// get height in cm
	/// </summary>
	/// <returns>height of camera in cm</returns>
	constexpr int GetHeight() const noexcept { return m_height; }

	/// <summary>
	/// get max view distance in cm
	/// </summary>
	/// <returns>max view distance in cm</returns>
	constexpr int GetMaxViewDistance() const noexcept { return m_maxViewDistance; }

	/// <summary>
	/// set max view distance in cm
	/// </summary>
	/// <param name="_maxViewDistance">max view distance in cm to set</param>
	constexpr void SetMaxViewDistance(int _maxViewDistance) noexcept { m_maxViewDistance = _maxViewDistance; }
	#pragma endregion

private:
	#pragma region primitive variable
	/// <summary>
	/// field of view
	/// </summary>
	SVector2 m_fov{ 120.0f, 60.0f };

	/// <summary>
	/// height of camera
	/// </summary>
	int m_height{ 100 };

	/// <summary>
	/// current angle
	/// </summary>
	float m_angle{ 0.0f };

	/// <summary>
	/// maximum view distance in cm
	/// </summary>
	int m_maxViewDistance{ 5000 };
	#pragma endregion
};