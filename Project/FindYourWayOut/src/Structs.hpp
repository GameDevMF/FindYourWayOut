#pragma once

#pragma region include project
#include "Vector2.hpp"
#pragma endregion

/// <summary>
/// level config struct
/// </summary>
struct SLevelConfig
{
public:
	#pragma region primitive variable
	/// <summary>
	/// camera field of view
	/// </summary>
	SVector2 CameraFOV = SVector2(90.0f, 90.0f);

	/// <summary>
	/// camera max view distance in cm
	/// </summary>
	int CameraMaxViewDistance = 1000;

	/// <summary>
	/// tile size in cm
	/// </summary>
	int TileSizeCm = 200;

	/// <summary>
	/// if this is the last level
	/// </summary>
	bool LastLevel = false;

	/// <summary>
	/// time for this level in seconds
	/// </summary>
	int Time = 240;

	/// <summary>
	/// char that is used for walls
	/// </summary>
	char WallChar = '\0';

	/// <summary>
	/// char that is used for the player
	/// </summary>
	char PlayerChar = '\0';

	/// <summary>
	/// char that is used for the exit
	/// </summary>
	char ExitChar = '\0';
	#pragma endregion
};