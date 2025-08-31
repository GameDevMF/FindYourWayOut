#pragma region include project
#include "Enums.hpp"
#include "Player.hpp"
#include "ObjectManager.hpp"
#include "Renderer.hpp"
#include "Minimap.hpp"
#pragma endregion

Minimap::~Minimap()
{
	delete[] m_pCharInfo;
}

void Minimap::Init()
{
	m_minimapSize = ObjectManager::Get().GetLevelConfig().CameraMaxViewDistance / ObjectManager::Get().GetLevelConfig().TileSizeCm;

	// double the mini map size and decrease by 1 so that the player is in the middle
	m_minimapSize *= 2;
	m_minimapSize--;

	// max mini map is 19
	if (m_minimapSize > 19)
		m_minimapSize = 19;

	// create char info array by mini map size plus border
	if(m_pCharInfo)
		delete[] m_pCharInfo;
	m_pCharInfo = new CHAR_INFO[(m_minimapSize + 2) * (m_minimapSize + 2)];

	for (int y = 0; y < m_minimapSize + 2; y++)
	{
		for (int x = 0; x < m_minimapSize + 2; x++)
		{
			m_pCharInfo[y * (m_minimapSize + 2) + x].Char.UnicodeChar = ECHAR_TYPE::SOLID;

			// if border char set color to yellow, inside black
			if (y == 0 || x == 0 || x == m_minimapSize + 1 || y == m_minimapSize + 1)
				m_pCharInfo[y * (m_minimapSize + 2) + x].Attributes = ECHAR_COLOR::YELLOW;
			else
				m_pCharInfo[y * (m_minimapSize + 2) + x].Attributes = ECHAR_COLOR::BLACK;
		}
	}

	// set middle to blue for player
	m_pCharInfo[static_cast<int>(m_minimapSize * 0.5f + 1) * (m_minimapSize + 2) + static_cast<int>(m_minimapSize * 0.5f + 1)].Attributes = ECHAR_COLOR::FG_BLUE;

	m_tileSize = ObjectManager::Get().GetLevelConfig().TileSizeCm;
}

void Minimap::Update(float _deltaSeconds)
{
	m_checkLocation = m_pPlayer->GetLocation();

	m_checkLocation.X -= static_cast<int>(m_checkLocation.X) % m_tileSize;
	m_checkLocation.X += m_tileSize * 0.5f;
	m_checkLocation.X -= m_tileSize * static_cast<int>(m_minimapSize * 0.5f);

	m_checkLocation.Y -= static_cast<int>(m_checkLocation.Y) % m_tileSize;
	m_checkLocation.Y += m_tileSize * 0.5f;
	m_checkLocation.Y -= m_tileSize * static_cast<int>(m_minimapSize * 0.5f);

	for (int y = 1; y < m_minimapSize + 1; y++)
	{
		for (int x = 1; x < m_minimapSize + 1; x++)
		{
			if (m_checkLocation.X > 0.0f && m_checkLocation.Y > 0.0f)
			{
				// if check location hit wall set grey else black
				if(ObjectManager::Get().HitWall(m_checkLocation))
					m_pCharInfo[y * (m_minimapSize + 2) + x].Attributes = ECHAR_COLOR::GREY;
				else
					m_pCharInfo[y * (m_minimapSize + 2) + x].Attributes = ECHAR_COLOR::BLACK;
			}

			// if check location is under 0 set black
			else
			{
				m_pCharInfo[y * (m_minimapSize + 2) + x].Attributes = ECHAR_COLOR::BLACK;
			}

			m_checkLocation.X += m_tileSize;
		}

		// reset check location horizontal
		m_checkLocation.X -= m_tileSize * m_minimapSize;

		m_checkLocation.Y += m_tileSize;
	}

	// set middle to blue for player
	m_pCharInfo[static_cast<int>(m_minimapSize * 0.5f + 1) * (m_minimapSize + 2) + static_cast<int>(m_minimapSize * 0.5f + 1)].Attributes = ECHAR_COLOR::BLUE;
}

void Minimap::Render()
{
	Renderer::Get().RenderAtPos(m_pCharInfo, { 0.0f, 0.0f }, (m_minimapSize + 2), (m_minimapSize + 2));
}