#pragma once

#pragma region include project
#include "Enums.hpp"
#include "Vector2.hpp"
#pragma endregion

/// <summary>
/// base object class
/// </summary>
class Object
{
public:
	#pragma region constructor
	/// <summary>
	/// constructor
	/// </summary>
	Object() {}

	/// <summary>
	/// constructor
	/// </summary>
	/// <param name="_location">world location to set</param>
	Object(SVector2 _location) : m_location(_location) {}
	#pragma endregion

	#pragma region destructor
	/// <summary>
	/// destructor
	/// </summary>
	virtual ~Object() {}
	#pragma endregion

	#pragma region virtual method
	/// <summary>
	/// initialize object
	/// </summary>
	virtual void Init() = 0;

	/// <summary>
	/// update every frame
	/// </summary>
	/// <param name="_deltaSeconds">time since last frame</param>
	virtual void Update(float _deltaSeconds) = 0;

	/// <summary>
	/// render every frame
	/// </summary>
	virtual void Render() = 0;
	#pragma endregion

	#pragma region inline method
	/// <summary>
	/// get class type
	/// </summary>
	/// <returns>class type</returns>
	constexpr ECLASS_TYPE GetClassType() const noexcept { return m_classType; }

	/// <summary>
	/// get world location of object
	/// </summary>
	/// <returns></returns>
	inline SVector2 GetLocation() const { return m_location; }

	/// <summary>
	/// set world location
	/// </summary>
	/// <param name="_location">world location</param>
	inline void SetLocation(SVector2 _location) { m_location = _location; }
	#pragma endregion

protected:
	#pragma region variable
	/// <summary>
	/// type of class
	/// </summary>
	ECLASS_TYPE m_classType{ ECLASS_TYPE::OBJECT };

	/// <summary>
	/// world location of object
	/// </summary>
	SVector2 m_location{ 0.0f, 0.0f };
	#pragma endregion
};