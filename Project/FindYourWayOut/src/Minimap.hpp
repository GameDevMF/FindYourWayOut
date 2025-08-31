#pragma once

#pragma region system include
#include <windows.h>
#pragma endregion

#pragma region include project
#include "Object.hpp"
#pragma endregion

#pragma region forward declaration
class Player;
#pragma endregion

/// <summary>
/// mini map class
/// </summary>
class Minimap : public Object
{
public:
	#pragma region constructor
	/// <summary>
	/// constructor
	/// </summary>
	Minimap() : Object({0.0f, 0.0f}) {}
	#pragma endregion

	#pragma region destructor
	/// <summary>
	/// destructor
	/// </summary>
	~Minimap();
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
	/// set player reference
	/// </summary>
	/// <param name="_pPlayer">player reference</param>
	inline void SetPlayer(Player* _pPlayer) { m_pPlayer = _pPlayer; }
	#pragma endregion

private:
	#pragma region primitive variable
	/// <summary>
	/// tile size
	/// </summary>
	int m_tileSize = 0;

	/// <summary>
	/// size of the mini map
	/// </summary>
	int m_minimapSize = 0;
	#pragma endregion

	#pragma region variable
	/// <summary>
	/// check location for wall hit
	/// </summary>
	SVector2 m_checkLocation;
	#pragma endregion

	#pragma region pointer
	/// <summary>
	/// player reference
	/// </summary>
	Player* m_pPlayer = nullptr;

	/// <summary>
	/// mini map char info
	/// </summary>
	CHAR_INFO* m_pCharInfo = nullptr;
	#pragma endregion
};