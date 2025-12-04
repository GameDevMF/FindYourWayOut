#pragma region include project
#include "Input.hpp"
#include "Renderer.hpp"
#include "Game.hpp"
#include "SceneMenu.hpp"
#pragma endregion

SceneMenu::~SceneMenu()
{
	delete[] m_pStart;
	delete[] m_pWelcome;
}

void SceneMenu::Init()
{
	const char* welcome{ "Welcome to HELLAGE!" };
	const char* start{ "Press ENTER to start!" };

	m_pWelcome = new CHAR_INFO[19];
	m_pStart = new CHAR_INFO[21];

	m_location = { Renderer::Get().GetScreenWidth() * 0.5f - 10.0f, Renderer::Get().GetScreenHeight() * 0.5f };

	m_startTextPosition = { m_location.X - 1.0f, m_location.Y + 1.0f };

	for (int i{ 0 }; i < 19; i++)
	{
		m_pWelcome[i].Char.UnicodeChar = welcome[i];

		if (i < 11 || i > 17)
			m_pWelcome[i].Attributes = static_cast<unsigned short>(ECHAR_COLOR::FG_WHITE);
		else
			m_pWelcome[i].Attributes = static_cast<unsigned short>(ECHAR_COLOR::FG_RED);
	}

	for (int i{ 0 }; i < 21; i++)
	{
		m_pStart[i].Char.UnicodeChar = start[i];
		m_pStart[i].Attributes = static_cast<unsigned short>(ECHAR_COLOR::FG_GREY);
	}
}

void SceneMenu::Update(float _deltaSeconds)
{
	if (Input::IsKeyPressedThisFrame(VK_ESCAPE))
		Game::Get().StopGame();

	if (Input::IsKeyPressedThisFrame(VK_RETURN))
		Game::Get().SwitchScene(ESCENE_TYPE::LEVEL, 1);
}

void SceneMenu::Render()
{
	Renderer::Get().RenderAtPos(m_pWelcome, m_location, 19, 1);
	Renderer::Get().RenderAtPos(m_pStart, m_startTextPosition, 21, 1);
}