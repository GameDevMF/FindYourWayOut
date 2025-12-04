#pragma region include system
#define _USE_MATH_DEFINES
#include <math.h>
#pragma endregion

#pragma region include project
#include "Input.hpp"
#include "Game.hpp"
#include "ObjectManager.hpp"
#include "Player.hpp"
#pragma endregion

void Player::Init()
{
	m_radiantToDegreeCalculateValue = M_PI / 180.0f;

	if (!m_pCamera)
		m_pCamera = std::make_unique<Camera>(m_location);
	else
		m_pCamera->SetLocation(m_location);
	m_pCamera->SetFov(ObjectManager::Get().GetLevelConfig().CameraFOV);
	m_pCamera->SetMaxViewDistance(ObjectManager::Get().GetLevelConfig().CameraMaxViewDistance);
	m_pCamera->SetAngle(0.0f);

	if (!m_pMinimap)
		m_pMinimap = std::make_unique<Minimap>();
	m_pMinimap->SetPlayer(this);
	m_pMinimap->Init();

	m_timeLeft = ObjectManager::Get().GetLevelConfig().Time;

	if (!m_pHUD)
		m_pHUD = std::make_unique<HUD>();
	m_pHUD->Init();
	m_pHUD->UpdateTimeLeft(m_timeLeft);
}

void Player::Update(float _deltaSeconds)
{
	if (Input::IsKeyPressedThisFrame(VK_ESCAPE))
	{
		Game::Get().SwitchScene(ESCENE_TYPE::MENU);
		return;
	}

	m_timeLeft -= _deltaSeconds;
	m_pHUD->UpdateTimeLeft(m_timeLeft);

	if (m_timeLeft <= 0.0f)
	{
		Game::Get().SwitchScene(ESCENE_TYPE::GAME_OVER);
		return;
	}

	m_pMinimap->Update(_deltaSeconds);

	// rotate camera by right and left arrow keys
	if (Input::IsKeyPressed(VK_RIGHT))
		m_pCamera->SetAngle(m_pCamera->GetAngle() - m_rotationSpeed * _deltaSeconds);
	if (Input::IsKeyPressed(VK_LEFT))
		m_pCamera->SetAngle(m_pCamera->GetAngle() + m_rotationSpeed * _deltaSeconds);

	// calculate camera angle radiant
	m_cameraAngleRadiant = m_pCamera->GetAngle() * m_radiantToDegreeCalculateValue;

	m_forward = { static_cast<float>(sin(m_cameraAngleRadiant)), static_cast<float>(cos(m_cameraAngleRadiant)) };

	const SVector2 locationForwardNext{ m_location + (m_forward * (m_movementSpeed * _deltaSeconds)) };
	const SVector2 locationBackwardNext{ m_location - (m_forward * (m_movementSpeed * _deltaSeconds)) };

	if (Input::IsKeyPressed(VK_UP))
	{
		if(!ObjectManager::Get().HitWall(locationForwardNext))
			m_location = locationForwardNext;
	}

	if (Input::IsKeyPressed(VK_DOWN))
	{
		if (!ObjectManager::Get().HitWall(locationBackwardNext))
			m_location = locationBackwardNext;
	}

	m_pCamera->SetLocation(m_location);

	if (m_exitLocation.X > 0.0f && m_exitLocation.Y > 0.0f)
	{
		// if distance to exit location is lower than 50cm switch scene
		if ((m_exitLocation - m_location).LengthSquared() < 2500.0f)
		{
			if (ObjectManager::Get().GetLevelConfig().LastLevel)
			{
				Game::Get().SwitchScene(ESCENE_TYPE::GAME_WIN);
			}
			else
			{
				Game::Get().SwitchScene(ESCENE_TYPE::LEVEL, Game::Get().GetLevel() + 1);

				// set new level on ui
				m_pHUD->UpdateLevel();
			}
		}
	}
}

void Player::Render()
{
	m_pMinimap->Render();
	m_pHUD->Render();
}