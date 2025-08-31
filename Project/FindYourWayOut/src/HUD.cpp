#pragma region include system
#include <string>
#pragma endregion

#pragma region include project
#include "Enums.hpp"
#include "Game.hpp"
#include "Renderer.hpp"
#include "HUD.hpp"
#pragma endregion

HUD::~HUD()
{
	delete[] m_pCharInfo;
}

void HUD::Init()
{
	int hudWidth = 12;
	int hudHeight = 4;

	m_location.Y = Renderer::Get().GetScreenHeight() - 4.0f;

	if(m_pCharInfo)
		delete[] m_pCharInfo;
	m_pCharInfo = new CHAR_INFO[hudWidth * hudHeight];

	for (int y = 0; y < hudHeight; y++)
	{
		for (int x = 0; x < hudWidth; x++)
		{
			m_pCharInfo[y * hudWidth + x].Char.UnicodeChar = ECHAR_TYPE::SOLID;

			// set border color of hud yellow, inside black
			if (y == 0 || y == hudHeight - 1 || x == 0 || x == hudWidth - 1)
				m_pCharInfo[y * hudWidth + x].Attributes = ECHAR_COLOR::YELLOW;
			else
				m_pCharInfo[y * hudWidth + x].Attributes = ECHAR_COLOR::BLACK;
		}
	}

	const char* level = "LEVEL:   1";
	const char* time = "TIME:    0";

	for (int i = 0; i < hudWidth - 2; i++)
	{
		m_pCharInfo[1 * hudWidth + i + 1].Char.UnicodeChar = level[i];
		m_pCharInfo[1 * hudWidth + i + 1].Attributes = ECHAR_COLOR::FG_WHITE;
		m_pCharInfo[2 * hudWidth + i + 1].Char.UnicodeChar = time[i];
		m_pCharInfo[2 * hudWidth + i + 1].Attributes = ECHAR_COLOR::FG_WHITE;
	}

	UpdateLevel();
}

void HUD::Render()
{
	Renderer::Get().RenderAtPos(m_pCharInfo, m_location, 12, 4);
}

void HUD::UpdateLevel()
{
	int currentNumber = Game::Get().GetLevel() % 10;
	std::string numberAsString = std::to_string(currentNumber);

	m_pCharInfo[1 * 12 + 10].Char.UnicodeChar = numberAsString.c_str()[0];

	// if current level has more than 1 chars
	if (Game::Get().GetLevel() > 9)
	{
		currentNumber = (Game::Get().GetLevel() % 100) * 0.1f;
		numberAsString = std::to_string(currentNumber);
		m_pCharInfo[1 * 12 + 9].Char.UnicodeChar = numberAsString.c_str()[0];
	}
}

void HUD::UpdateTimeLeft(const int _time)
{
	int currentNumber = _time % 10;
	std::string numberAsString = std::to_string(currentNumber);

	// set third time char
	m_pCharInfo[2 * 12 + 10].Char.UnicodeChar = numberAsString.c_str()[0];

	// if current time has more than 1 chars
	if (_time > 9)
	{
		currentNumber = (_time % 100) * 0.1f;
		numberAsString = std::to_string(currentNumber);
		m_pCharInfo[2 * 12 + 9].Char.UnicodeChar = numberAsString.c_str()[0];

		// if current time has more than 2 chars
		if (_time > 99)
		{
			currentNumber = _time * 0.01f;
			numberAsString = std::to_string(currentNumber);
			m_pCharInfo[2 * 12 + 8].Char.UnicodeChar = numberAsString.c_str()[0];
		}
		// if current time has not more than 2 chars
		else
		{
			// set first time char
			m_pCharInfo[2 * 12 + 8].Char.UnicodeChar = ' ';
		}
	}
	// if current time has not more than 1 chars
	else
	{
		// set second time char
		m_pCharInfo[2 * 12 + 9].Char.UnicodeChar = ' ';
	}
}