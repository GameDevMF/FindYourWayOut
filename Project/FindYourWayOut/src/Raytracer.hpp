#pragma once

#pragma region system include
#include <windows.h>
#pragma endregion

#pragma region include project
#include "Object.hpp"
#pragma endregion

/// <summary>
/// class to handle rendering the 3D world
/// </summary>
class Raytracer : public Object
{
public:
	#pragma region singleton
	/// <summary>
	/// get game instance
	/// </summary>
	/// <returns>game instance</returns>
	static Raytracer& Get()
	{
		static Raytracer instance;
		return instance;
	}
	#pragma endregion

	#pragma region override method
	/// <summary>
	/// initialize object
	/// </summary>
	virtual void Init() override { }

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

	#pragma region inline method
	/// <summary>
	/// set exit location
	/// </summary>
	/// <param name="_location">exit location to set</param>
	inline void SetExitLocation(const SVector2 _location) { m_exitLocation = _location; }
	#pragma endregion

	#pragma region method
	/// <summary>
	/// prepare raytracer before rendering
	/// </summary>
	void Prepare();
	#pragma endregion

private:
	#pragma region constructor
	/// <summary>
	/// constructor
	/// </summary>
	Raytracer() {}
	#pragma endregion

	#pragma region destructor
	/// <summary>
	/// destructor
	/// </summary>
	~Raytracer()
	{
		delete[] m_pCurrentLineCharInfo;
		delete[] m_pLineDistance;
		delete[] m_pWallStartHeight;
	}
	#pragma endregion

	#pragma region primitive variable
	/// <summary>
	/// if raytracer needed variables are set
	/// </summary>
	bool m_prepared = false;

	/// <summary>
	/// check horizontal walls
	/// </summary>
	bool m_isHorizontalCheck = true;

	/// <summary>
	/// start position of wall at screen height
	/// </summary>
	int m_renderStartPos = 0;

	/// <summary>
	/// render height of wall at screen height
	/// </summary>
	int m_renderHeight = 0;

	/// <summary>
	/// camera x location at full integer value
	/// </summary>
	int m_cameraIntX = static_cast<int>(m_cameraLocation.X);

	/// <summary>
	/// camera y location at full integer value
	/// </summary>
	int m_cameraIntY = static_cast<int>(m_cameraLocation.Y);

	/// <summary>
	/// screen height in chars
	/// </summary>
	int m_screenHeight = 0;

	/// <summary>
	/// screen width in chars
	/// </summary>
	int m_screenWidth = 0;

	/// <summary>
	/// size of a tile in the level
	/// </summary>
	int m_tileSize = 0;

	/// <summary>
	/// max view distance in cm
	/// </summary>
	int m_maxViewDistance = 0;

	/// <summary>
	/// how many steps the current trace has made
	/// </summary>
	int m_traceSteps = 0;

	/// <summary>
	/// max steps for one trace
	/// </summary>
	int m_traceStepsMax = 0;

	/// <summary>
	/// max wall height from bottom to start in cm
	/// </summary>
	int m_maxWallHeight = 0;

	/// <summary>
	/// angle between camera and exit location to check if in view
	/// </summary>
	float m_angleBetweenCameraAndExitLocation = 0.0f;

	/// <summary>
	/// previous angle between camera and exit location to check if in view
	/// </summary>
	float m_previousAngleBetweenCameraAndExitLocation = 0.0f;

	/// <summary>
	/// half field of view horizontal
	/// </summary>
	float m_halfHorizontalFov = 0.0f;

	/// <summary>
	/// current trace line angle in degree
	/// </summary>
	float m_currentTraceLineAngleDegree = 0.0f;

	/// <summary>
	/// current trace line angle in radiant
	/// </summary>
	float m_currentTraceLineAngleRadiant = 0.0f;

	/// <summary>
	/// angle per trace line in degree
	/// </summary>
	float m_anglePerLineDegree = 0.0f;

	/// <summary>
	/// angle to wall
	/// </summary>
	float m_traceAngleToWall = 0.0f;

	/// <summary>
	/// distance to camera from hit location
	/// </summary>
	float m_distanceToCamera = 0.0f;

	/// <summary>
	/// height in cm at hit location depending on field of view vertical
	/// </summary>
	float m_heightFovVerticalAtDistanceToCamera = 0.0f;

	/// <summary>
	/// value to calculate height at hit location
	/// </summary>
	float m_distanceToCameraToHeightCalculateValue = 0.0f;

	/// <summary>
	/// distance to wall from top at hit location from height
	/// </summary>
	float m_distanceToWallFromFovHeight = 0.0f;

	/// <summary>
	/// radiant calculate value
	/// </summary>
	float m_radiantToDegreeCalculateValue = 0.0f;
	#pragma endregion

	#pragma region variable
	/// <summary>
	/// current trace line forward vector
	/// </summary>
	SVector2 m_currentTraceLineForward;

	/// <summary>
	/// forward vector of horizontal wall check
	/// </summary>
	SVector2 m_horizontalCheckForward;

	/// <summary>
	/// forward vector of vertical wall check
	/// </summary>
	SVector2 m_verticalCheckForward;

	/// <summary>
	/// location to check at horizontal wall
	/// </summary>
	SVector2 m_horizontalCheckLocation;

	/// <summary>
	/// location to check at vertical wall
	/// </summary>
	SVector2 m_verticalCheckLocation;

	/// <summary>
	/// camera location
	/// </summary>
	SVector2 m_cameraLocation;

	/// <summary>
	/// exit location
	/// </summary>
	SVector2 m_exitLocation;
	#pragma endregion

	#pragma region pointer
	/// <summary>
	/// height in pixel where wall starts at every horizontal line
	/// </summary>
	int* m_pWallStartHeight = nullptr;

	/// <summary>
	/// distance to camera of every horizontal line
	/// </summary>
	float* m_pLineDistance = nullptr;

	/// <summary>
	/// vertical screen line char info
	/// </summary>
	CHAR_INFO* m_pCurrentLineCharInfo = nullptr;
	#pragma endregion

	#pragma region method
	/// <summary>
	/// calculate current trace line forward vector
	/// </summary>
	void CalculateCurrentTraceLineForwardVector();

	/// <summary>
	/// calculate horizontal check forward vector
	/// </summary>
	void CalculateHorizontalCheckForwardVector();

	/// <summary>
	/// calculate vertical check forward vector
	/// </summary>
	void CalculateVerticalCheckForwardVector();

	/// <summary>
	/// set first check locations of current trace line
	/// </summary>
	void SetFirstCheckLocations();

	/// <summary>
	/// calculate check steps horizontal and vertical
	/// </summary>
	void CalculateCheckSteps();
	
	/// <summary>
	/// trace along line to calculate distance to camera if hit
	/// </summary>
	/// <param name="_currentLine">current screen vertical line</param>
	void TraceAlongLine(const int& _currentLine);

	/// <summary>
	/// if current check hits a wall
	/// </summary>
	/// <returns>current check hits a wall</returns>
	bool IsCheckHitsWall();

	/// <summary>
	/// set all chars in current height line
	/// </summary>
	/// <param name="_currentLine">current screen vertical line</param>
	void SetHeightLineChars(const int& _currentLine);

	/// <summary>
	/// trace from camera to exit location
	/// </summary>
	void TraceToExitLocation();

	/// <summary>
	/// fill ground chars to closest wall pixel
	/// </summary>
	void FillGroundToClosestWallPixel();
	#pragma endregion
};