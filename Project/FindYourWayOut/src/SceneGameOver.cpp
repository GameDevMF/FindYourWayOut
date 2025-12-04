#pragma region include project
#include "Input.hpp"
#include "Renderer.hpp"
#include "Game.hpp"
#include "SceneGameOver.hpp"
#pragma endregion

SceneGameOver::~SceneGameOver()
{
	delete[] m_pMenu;
	delete[] m_pGameOver;
}

void SceneGameOver::Init()
{
	const char* gameOver{ "Game Over" };
	const char* menu{ "Press ESC to Menu!" };

	m_pGameOver = new CHAR_INFO[9];
	m_pMenu = new CHAR_INFO[18];

	m_location = { Renderer::Get().GetScreenWidth() * 0.5f - 5.0f, Renderer::Get().GetScreenHeight() * 0.5f };

	m_startTextLocation = { m_location.X - 4.0f, m_location.Y + 1.0f };

	for (int i{ 0 }; i < 9; i++)
	{
		m_pGameOver[i].Char.UnicodeChar = gameOver[i];
		m_pGameOver[i].Attributes = static_cast<unsigned short>(ECHAR_COLOR::FG_RED);
	}

	for (int i{ 0 }; i < 18; i++)
	{
		m_pMenu[i].Char.UnicodeChar = menu[i];
		m_pMenu[i].Attributes = static_cast<unsigned short>(ECHAR_COLOR::FG_WHITE);
	}
}

void SceneGameOver::Update(float _deltaSeconds)
{
	if (Input::IsKeyPressedThisFrame(VK_ESCAPE))
		Game::Get().SwitchScene(ESCENE_TYPE::MENU);
}

void SceneGameOver::Render()
{
	Renderer::Get().RenderAtPos(m_pGameOver, m_location, 9, 1);
	Renderer::Get().RenderAtPos(m_pMenu, m_startTextLocation, 18, 1);
}