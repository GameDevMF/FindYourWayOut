#pragma region include project
#include "Raytracer.hpp"
#include "Enums.hpp"
#include "Camera.hpp"
#include "ObjectManager.hpp"
#include "Renderer.hpp"
#pragma endregion

#pragma region include system
#define _USE_MATH_DEFINES
#include <math.h>
#pragma endregion

void Raytracer::Render()
{
	if (!m_prepared)
		return;

	// start trace angle at end of field of view horizontal
	m_currentTraceLineAngleDegree = ObjectManager::Get().GetPlayer().GetCamera().GetAngle() + m_halfHorizontalFov;

	m_cameraLocation = ObjectManager::Get().GetPlayer().GetCamera().GetLocation();

	// save current camera location full integer for better calculation
	m_cameraIntX = static_cast<int>(m_cameraLocation.X) % m_tileSize;
	m_cameraIntY = static_cast<int>(m_cameraLocation.Y) % m_tileSize;

	m_traceSteps = 0;
	m_maxWallHeight = 0;

	for (int verticalScreenLine = 0; verticalScreenLine < m_screenWidth; verticalScreenLine++)
	{
		CalculateCurrentTraceLineForwardVector();

		CalculateHorizontalCheckForwardVector();

		CalculateVerticalCheckForwardVector();

		SetFirstCheckLocations();

		CalculateCheckSteps();

		TraceAlongLine(verticalScreenLine);

		SetHeightLineChars(verticalScreenLine);

		// render current screen height line
		Renderer::Get().RenderAtPos(m_pCurrentLineCharInfo, SVector2(verticalScreenLine, 0.0f), 1, m_screenHeight);

		// increase current trace line angle
		m_currentTraceLineAngleDegree -= m_anglePerLineDegree;
	}

	FillGroundToClosestWallPixel();

	TraceToExitLocation();
}

void Raytracer::Prepare()
{
	m_screenWidth = Renderer::Get().GetScreenWidth();
	m_screenHeight = Renderer::Get().GetScreenHeight();

	// half horizontal fov prevents for calculation every frame
	m_halfHorizontalFov = ObjectManager::Get().GetPlayer().GetCamera().GetFov().X * 0.5f;

	// angle for a single line depending on fov
	m_anglePerLineDegree = ObjectManager::Get().GetPlayer().GetCamera().GetFov().X / m_screenWidth;

	// calculate value to convert degree to radiant
	m_radiantToDegreeCalculateValue = M_PI / 180.0f;

	// calculate value to calculate distance from camera to height
	m_distanceToCameraToHeightCalculateValue = tan((ObjectManager::Get().GetPlayer().GetCamera().GetFov().Y * 0.5f) * m_radiantToDegreeCalculateValue) * 2.0f;

	m_tileSize = ObjectManager::Get().GetLevelConfig().TileSizeCm;
	m_maxViewDistance = ObjectManager::Get().GetPlayer().GetCamera().GetMaxViewDistance();

	// calculate max steps for one trace
	m_traceStepsMax = ObjectManager::Get().GetPlayer().GetCamera().GetMaxViewDistance() / m_tileSize * 2.0f;

	if (!m_pCurrentLineCharInfo)
	{
		m_pCurrentLineCharInfo = new CHAR_INFO[m_screenHeight];

		for (int i = 0; i < m_screenHeight; i++)
			m_pCurrentLineCharInfo[i].Char.UnicodeChar = ECHAR_TYPE::SOLID;
	}

	if (!m_pLineDistance)
		m_pLineDistance = new float[m_screenWidth];
	
	if (!m_pWallStartHeight)
		m_pWallStartHeight = new int[m_screenWidth];

	m_prepared = true;
}

void Raytracer::CalculateCurrentTraceLineForwardVector()
{
	m_currentTraceLineAngleRadiant = m_radiantToDegreeCalculateValue * m_currentTraceLineAngleDegree;

	m_currentTraceLineForward.X = std::sin(m_currentTraceLineAngleRadiant);
	m_currentTraceLineForward.Y = std::cos(m_currentTraceLineAngleRadiant);
}

void Raytracer::CalculateHorizontalCheckForwardVector()
{
	if (m_currentTraceLineForward.X < 0.0f)
	{
		m_horizontalCheckForward.X = -m_cameraIntX - 1;

		// if current trace angle line degree is exactly 270 trace only horizontal
		if (m_currentTraceLineAngleDegree == 270.0f)
		{
			m_horizontalCheckForward.Y = 0.0f;
		}
		else
		{
			if (m_currentTraceLineForward.Y < 0.0f)
			{
				m_traceAngleToWall = 270.0f - m_currentTraceLineAngleDegree;
			}
			else
			{
				m_traceAngleToWall = m_currentTraceLineAngleDegree - 270.0f;
			}

			// negate horizontal forward y check depending on horizontal check forward x and trace line forward y
			if ((m_horizontalCheckForward.X < 0.0f && m_currentTraceLineForward.Y > 0.0f) || (m_horizontalCheckForward.X > 0.0f && m_currentTraceLineForward.Y < 0.0f))
				m_horizontalCheckForward.Y = -m_horizontalCheckForward.X * tan(m_traceAngleToWall * m_radiantToDegreeCalculateValue);
			else
				m_horizontalCheckForward.Y = m_horizontalCheckForward.X * tan(m_traceAngleToWall * m_radiantToDegreeCalculateValue);
		}
	}
	else
	{
		m_horizontalCheckForward.X = m_tileSize - m_cameraIntX;

		// if current trace angle line degree is exactly 90 trace only horizontal
		if (m_currentTraceLineAngleDegree == 90.0f)
		{
			m_horizontalCheckForward.Y = 0.0f;
		}
		else
		{
			if (m_currentTraceLineForward.Y < 0.0f)
			{
				m_traceAngleToWall = m_currentTraceLineAngleDegree - 90.0f;
			}
			else
			{
				m_traceAngleToWall = 90.0f - m_currentTraceLineAngleDegree;
			}

			// negate horizontal forward y check depending on horizontal check forward x and trace line forward y
			if ((m_horizontalCheckForward.X < 0.0f && m_currentTraceLineForward.Y > 0.0f) || (m_horizontalCheckForward.X > 0.0f && m_currentTraceLineForward.Y < 0.0f))
				m_horizontalCheckForward.Y = -m_horizontalCheckForward.X * tan(m_traceAngleToWall * m_radiantToDegreeCalculateValue);
			else
				m_horizontalCheckForward.Y = m_horizontalCheckForward.X * tan(m_traceAngleToWall * m_radiantToDegreeCalculateValue);
		}
	}
}

void Raytracer::CalculateVerticalCheckForwardVector()
{
	if (m_currentTraceLineForward.Y < 0.0f)
	{
		m_verticalCheckForward.Y = -m_cameraIntY - 1;

		// if current trace angle line degree is exactly 270 trace only vertical
		if (m_currentTraceLineAngleDegree == 180.0f)
		{
			m_verticalCheckForward.X = 0.0f;
		}
		else
		{
			if (m_currentTraceLineForward.X < 0.0f)
			{
				m_traceAngleToWall = m_currentTraceLineAngleDegree - 180.0f;
			}
			else
			{
				m_traceAngleToWall = 180.0f - m_currentTraceLineAngleDegree;
			}

			// negate horizontal forward x check depending on horizontal check forward y and trace line forward y
			if ((m_verticalCheckForward.Y < 0.0f && m_currentTraceLineForward.X > 0.0f) || (m_verticalCheckForward.Y > 0.0f && m_currentTraceLineForward.X < 0.0f))
				m_verticalCheckForward.X = -m_verticalCheckForward.Y * tan(m_traceAngleToWall * m_radiantToDegreeCalculateValue);
			else
				m_verticalCheckForward.X = m_verticalCheckForward.Y * tan(m_traceAngleToWall * m_radiantToDegreeCalculateValue);
		}
	}
	else
	{
		m_verticalCheckForward.Y = m_tileSize - m_cameraIntY;

		// if current trace angle line degree is exactly 0 trace only vertical
		if (m_currentTraceLineAngleDegree == 0.0f)
		{
			m_verticalCheckForward.X = 0.0f;
		}
		else
		{
			if (m_currentTraceLineForward.X < 0.0f)
			{
				m_traceAngleToWall = 360.0f - m_currentTraceLineAngleDegree;
			}
			else
			{
				m_traceAngleToWall = m_currentTraceLineAngleDegree;
			}

			// negate horizontal forward x check depending on horizontal check forward y and trace line forward y
			if ((m_verticalCheckForward.Y < 0.0f && m_currentTraceLineForward.X > 0.0f) || (m_verticalCheckForward.Y > 0.0f && m_currentTraceLineForward.X < 0.0f))
				m_verticalCheckForward.X = -m_verticalCheckForward.Y * tan(m_traceAngleToWall * m_radiantToDegreeCalculateValue);
			else
				m_verticalCheckForward.X = m_verticalCheckForward.Y * tan(m_traceAngleToWall * m_radiantToDegreeCalculateValue);
		}
	}
}

void Raytracer::SetFirstCheckLocations()
{
	if (m_horizontalCheckForward.Y == 0.0f && m_horizontalCheckForward.X != 0.0f)
	{
		m_verticalCheckForward.X = 0.0f;
		m_verticalCheckForward.Y = 0.0f;
	}
	else if (m_verticalCheckForward.X == 0.0f && m_verticalCheckForward.Y != 0.0f)
	{
		m_horizontalCheckForward.X = 0.0f;
		m_horizontalCheckForward.Y = 0.0f;
	}

	m_horizontalCheckLocation = m_cameraLocation + m_horizontalCheckForward;
	m_verticalCheckLocation = m_cameraLocation + m_verticalCheckForward;
}

void Raytracer::CalculateCheckSteps()
{
	if (m_verticalCheckForward.Y != 0.0f)
		m_horizontalCheckForward.Y = m_tileSize / abs(m_horizontalCheckForward.X) * m_horizontalCheckForward.Y;

	if (m_horizontalCheckForward.X > 0.0f)
		m_horizontalCheckForward.X = m_tileSize;
	else if (m_horizontalCheckForward.X < 0.0f)
		m_horizontalCheckForward.X = -m_tileSize;

	if (m_verticalCheckForward.X != 0.0f)
		m_verticalCheckForward.X = m_tileSize / abs(m_verticalCheckForward.Y) * m_verticalCheckForward.X;

	if (m_verticalCheckForward.Y > 0.0f)
		m_verticalCheckForward.Y = m_tileSize;
	else if (m_verticalCheckForward.Y < 0.0f)
		m_verticalCheckForward.Y = -m_tileSize;
}

void Raytracer::TraceAlongLine(const int& _currentLine)
{
	m_distanceToCamera = 0.0f;
	m_traceSteps = 0;

	while (m_distanceToCamera < m_maxViewDistance)
	{
		m_traceSteps++;

		if (m_traceSteps > m_traceStepsMax)
			break;

		if (IsCheckHitsWall())
			break;
	}

	if (m_distanceToCamera < m_maxViewDistance)
		m_pLineDistance[_currentLine] = m_distanceToCamera;
	else
		m_pLineDistance[_currentLine] = 0.0f;
}

bool Raytracer::IsCheckHitsWall()
{
	// check horizontal if horizontal check location to camera is shorter than vertical check location to camera
	m_isHorizontalCheck = (m_verticalCheckForward.X == 0.0f && m_verticalCheckForward.Y == 0.0f) ||
		((m_horizontalCheckLocation - m_cameraLocation).Length() < (m_verticalCheckLocation - m_cameraLocation).Length());

	if (m_isHorizontalCheck && m_horizontalCheckLocation.X > 0.0f && m_horizontalCheckLocation.Y > 0.0f)
	{
		if ((m_horizontalCheckForward.X != 0.0f && m_horizontalCheckForward.Y != 0.0f) ||
			(m_horizontalCheckForward.X == 0.0f && m_horizontalCheckForward.Y != 0.0f) ||
			(m_horizontalCheckForward.X != 0.0f && m_horizontalCheckForward.Y == 0.0f))
		{
			m_distanceToCamera = (m_horizontalCheckLocation - m_cameraLocation).Length();

			if (ObjectManager::Get().HitWall(m_horizontalCheckLocation))
				return true;

			// increase horizontal check location one tile size steps horizontal forward
			m_horizontalCheckLocation = m_horizontalCheckLocation + m_horizontalCheckForward;
		}
	}
	else if (m_verticalCheckLocation.X > 0.0f && m_verticalCheckLocation.Y > 0.0f)
	{
		if ((m_verticalCheckForward.X != 0.0f && m_verticalCheckForward.Y != 0.0f) ||
			(m_verticalCheckForward.X == 0.0f && m_verticalCheckForward.Y != 0.0f) ||
			(m_verticalCheckForward.X != 0.0f && m_verticalCheckForward.Y == 0.0f))
		{
			m_distanceToCamera = (m_verticalCheckLocation - m_cameraLocation).Length();

			if (ObjectManager::Get().HitWall(m_verticalCheckLocation))
				return true;

			// increase vertical check location one tile size steps vertical forward
			m_verticalCheckLocation = m_verticalCheckLocation + m_verticalCheckForward;
		}
	}

	return false;
}

void Raytracer::SetHeightLineChars(const int& _currentLine)
{
	if (m_distanceToCamera && m_distanceToCamera < m_maxViewDistance)
	{
		// height of field of view vertical at distance
		m_heightFovVerticalAtDistanceToCamera = m_distanceToCamera * m_distanceToCameraToHeightCalculateValue;

		// fill whole line if height is higher than room height
		if (m_heightFovVerticalAtDistanceToCamera <= 200.0f)
		{
			m_pWallStartHeight[_currentLine] = m_screenHeight;

			for (int i = 0; i < m_screenHeight; i++)
			{
				m_pCurrentLineCharInfo[i].Char.UnicodeChar = ECHAR_TYPE::SOLID;
				m_pCurrentLineCharInfo[i].Attributes = ECHAR_COLOR::CYAN;
			}
		}
		else
		{
			// calculate distance from height of field of view to half wall height in cm
			m_distanceToWallFromFovHeight = (m_heightFovVerticalAtDistanceToCamera - 200.0f) * 0.5f;

			// calculate distance from height of field of view to half wall height in screen pixel
			m_distanceToWallFromFovHeight = (m_distanceToWallFromFovHeight / m_heightFovVerticalAtDistanceToCamera) * m_screenHeight;

			// calculate start pos and height at screen
			m_renderStartPos = static_cast<int>(m_distanceToWallFromFovHeight);
			m_renderHeight = static_cast<int>(m_screenHeight - m_distanceToWallFromFovHeight);

			m_pWallStartHeight[_currentLine] = m_renderStartPos;

			if (m_renderStartPos > m_maxWallHeight)
				m_maxWallHeight = m_renderStartPos;

			for (int i = 0; i < m_screenHeight; i++)
			{
				if (i >= m_renderStartPos && i <= m_renderHeight)
				{
					if (m_distanceToCamera <= m_tileSize * 2.0f)
						m_pCurrentLineCharInfo[i].Attributes = ECHAR_COLOR::CYAN;
					else if (m_distanceToCamera <= m_tileSize * 4.0f)
						m_pCurrentLineCharInfo[i].Attributes = ECHAR_COLOR::BLUE;
					else
						m_pCurrentLineCharInfo[i].Attributes = ECHAR_COLOR::DARK_BLUE;
				}

				// if current char is below or above calculated wall
				else
				{
					if (i > m_screenHeight * 0.8f - 2)
						m_pCurrentLineCharInfo[i].Attributes = ECHAR_COLOR::GREEN;
					else if (i > m_screenHeight * 0.6f - 2)
						m_pCurrentLineCharInfo[i].Attributes = ECHAR_COLOR::DARK_GREEN;
					else if (i > m_screenHeight * 0.5f - 2)
						m_pCurrentLineCharInfo[i].Attributes = ECHAR_COLOR::BG_DARK_GREEN;
					else if (i > m_screenHeight * 0.4f)
						m_pCurrentLineCharInfo[i].Attributes = ECHAR_COLOR::BG_GREY;
					else if (i > m_screenHeight * 0.2f)
						m_pCurrentLineCharInfo[i].Attributes = ECHAR_COLOR::DARK_GREY;
					else
						m_pCurrentLineCharInfo[i].Attributes = ECHAR_COLOR::GREY;
				}
			}
		}
	}
	// if distance to camera is 0 or higher than max view distance
	else
	{
		m_pWallStartHeight[_currentLine] = -1;

		for (int i = 0; i < m_screenHeight; i++)
		{
			if (i > m_screenHeight * 0.8f - 2)
				m_pCurrentLineCharInfo[i].Attributes = ECHAR_COLOR::GREEN;
			else if (i > m_screenHeight * 0.6f - 2)
				m_pCurrentLineCharInfo[i].Attributes = ECHAR_COLOR::DARK_GREEN;
			else if (i > m_screenHeight * 0.5f - 2)
				m_pCurrentLineCharInfo[i].Attributes = ECHAR_COLOR::BG_DARK_GREEN;
			else if (i > m_screenHeight * 0.4f)
				m_pCurrentLineCharInfo[i].Attributes = ECHAR_COLOR::BG_GREY;
			else if (i > m_screenHeight * 0.2f)
				m_pCurrentLineCharInfo[i].Attributes = ECHAR_COLOR::DARK_GREY;
			else
				m_pCurrentLineCharInfo[i].Attributes = ECHAR_COLOR::GREY;
		}
	}
}

void Raytracer::TraceToExitLocation()
{
	// if distance from camera to exit location is higher than view distance return
	if ((m_cameraLocation - m_exitLocation).LengthSquared() >
		(ObjectManager::Get().GetLevelConfig().CameraMaxViewDistance * ObjectManager::Get().GetLevelConfig().CameraMaxViewDistance))
		return;

	// calculate camera angle to check
	m_currentTraceLineAngleRadiant = m_radiantToDegreeCalculateValue * ObjectManager::Get().GetPlayer().GetCamera().GetAngle();

	m_currentTraceLineForward.X = std::sin(m_currentTraceLineAngleRadiant);
	m_currentTraceLineForward.Y = std::cos(m_currentTraceLineAngleRadiant);
	m_currentTraceLineForward.Normalize();

	m_angleBetweenCameraAndExitLocation = m_currentTraceLineForward.Dot((m_exitLocation - m_cameraLocation).Normalized());

	// if not in view return
	if (m_angleBetweenCameraAndExitLocation < (ObjectManager::Get().GetLevelConfig().CameraFOV.X * 0.5f) * m_radiantToDegreeCalculateValue)
		return;

	// start trace angle at end of field of view horizontal
	m_currentTraceLineAngleDegree = ObjectManager::Get().GetPlayer().GetCamera().GetAngle() + m_halfHorizontalFov;

	m_currentTraceLineAngleRadiant = m_radiantToDegreeCalculateValue * m_currentTraceLineAngleDegree;

	m_currentTraceLineForward.X = std::sin(m_currentTraceLineAngleRadiant);
	m_currentTraceLineForward.Y = std::cos(m_currentTraceLineAngleRadiant);
	m_currentTraceLineForward.Normalize();

	m_previousAngleBetweenCameraAndExitLocation = m_currentTraceLineForward.Dot((m_exitLocation - m_cameraLocation).Normalized());

	// check every vertical screen line with one offset because it has to check whether the last dot product was higher
	for (int verticalScreenLine = 1; verticalScreenLine < m_screenWidth + 1; verticalScreenLine++)
	{
		m_currentTraceLineAngleDegree -= m_anglePerLineDegree;

		m_currentTraceLineAngleRadiant = m_radiantToDegreeCalculateValue * m_currentTraceLineAngleDegree;

		m_currentTraceLineForward.X = std::sin(m_currentTraceLineAngleRadiant);
		m_currentTraceLineForward.Y = std::cos(m_currentTraceLineAngleRadiant);
		m_currentTraceLineForward.Normalize();

		m_distanceToCamera = (m_exitLocation - m_cameraLocation).Length();

		m_angleBetweenCameraAndExitLocation = m_currentTraceLineForward.Dot((m_exitLocation - m_cameraLocation).Normalized());

		// if last dot product was higher, than this was the one which points closest to the exit or the last trace
		if (m_angleBetweenCameraAndExitLocation < m_previousAngleBetweenCameraAndExitLocation || verticalScreenLine == m_screenWidth)
		{
			// calculate height of exit depending on distance to camera
			m_heightFovVerticalAtDistanceToCamera = m_screenHeight * 0.75f - (m_screenHeight * 0.75f *
				(m_distanceToCamera / ObjectManager::Get().GetLevelConfig().CameraMaxViewDistance));
			m_heightFovVerticalAtDistanceToCamera *= 0.5f;

			// height is only straight number
			if (static_cast<int>(m_heightFovVerticalAtDistanceToCamera) % 2 != 0)
				m_heightFovVerticalAtDistanceToCamera--;

			// if height is too low return
			if (m_heightFovVerticalAtDistanceToCamera < 2.0f)
				return;			

			m_distanceToWallFromFovHeight = m_heightFovVerticalAtDistanceToCamera;

			// fill height line from center to top and bottom depending on distance to camera
			for (int column = 0; column < m_distanceToWallFromFovHeight * 0.5f - 1.0f; column++)
			{
				if (column && verticalScreenLine - 1.0f - column >= 0.0f)
					if (m_pLineDistance[verticalScreenLine - 1 - column] == 0.0f || m_pLineDistance[verticalScreenLine - 1 - column] > m_distanceToCamera + column)
						for (int i = (m_screenHeight * 0.5f) - m_heightFovVerticalAtDistanceToCamera; i < m_screenHeight * 0.5f + m_heightFovVerticalAtDistanceToCamera; i++)
							Renderer::Get().RenderAtPos(ECHAR_TYPE::SOLID, ECHAR_COLOR::YELLOW, { verticalScreenLine - 1.0f - column, static_cast<float>(i) });
				
				if(verticalScreenLine - 1.0f + column < m_screenWidth)
					if (m_pLineDistance[verticalScreenLine - 1 + column] == 0.0f || m_pLineDistance[verticalScreenLine - 1 + column] > m_distanceToCamera + column)
						for (int i = (m_screenHeight * 0.5f) - m_heightFovVerticalAtDistanceToCamera; i < m_screenHeight * 0.5f + m_heightFovVerticalAtDistanceToCamera; i++)
							Renderer::Get().RenderAtPos(ECHAR_TYPE::SOLID, ECHAR_COLOR::YELLOW, { verticalScreenLine - 1.0f + column, static_cast<float>(i) });

				m_heightFovVerticalAtDistanceToCamera -= 2.0f;
			}			

			return;
		}

		// save current angle for next line
		m_previousAngleBetweenCameraAndExitLocation = m_angleBetweenCameraAndExitLocation;
	}
}

void Raytracer::FillGroundToClosestWallPixel()
{
	if (m_maxWallHeight == m_screenHeight || m_maxWallHeight <= m_screenHeight * 0.4f)
		return;

	for (int verticalScreenLine = 0; verticalScreenLine < m_screenWidth; verticalScreenLine++)
	{
		// only affect lines which are not full wall and wall starts in dark green area
		if(m_pWallStartHeight[verticalScreenLine] == m_screenHeight ||
			(m_pWallStartHeight[verticalScreenLine] <= m_screenHeight * 0.4f && (m_pWallStartHeight[verticalScreenLine] != -1)))
			continue;

		for (int i = m_screenHeight * 0.4f + 1; i < m_maxWallHeight; i++)
		{
			if (m_pWallStartHeight[verticalScreenLine] > i || m_pWallStartHeight[verticalScreenLine] == -1)
			{
				Renderer::Get().RenderAtPos(ECHAR_TYPE::SOLID, ECHAR_COLOR::DARK_GREY, SVector2(verticalScreenLine, i));
				Renderer::Get().RenderAtPos(ECHAR_TYPE::SOLID, ECHAR_COLOR::DARK_GREEN, SVector2(verticalScreenLine, m_screenHeight - i - 1));
			}
		}
	}
}