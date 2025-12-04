#pragma region include project
#include "Input.hpp"
#include "Renderer.hpp"
#include "Game.hpp"
#include "SceneGameWin.hpp"
#pragma endregion

SceneGameWin::~SceneGameWin()
{
	delete[] m_pMenu;
	delete[] m_pGameWin;
}

void SceneGameWin::Init()
{
	const char* gameWin{ "You Won!" };
	const char* menu{ "Press ESC to Menu!" };

	m_pGameWin = new CHAR_INFO[9];
	m_pMenu = new CHAR_INFO[18];

	m_location = { Renderer::Get().GetScreenWidth() * 0.5f - 4.0f, Renderer::Get().GetScreenHeight() * 0.5f };

	m_startTextLocation = { m_location.X - 5.0f, m_location.Y + 1.0f };

	for (int i{ 0 }; i < 8; i++)
	{
		m_pGameWin[i].Char.UnicodeChar = gameWin[i];
		m_pGameWin[i].Attributes = static_cast<unsigned short>(ECHAR_COLOR::FG_GREEN);
	}

	for (int i{ 0 }; i < 18; i++)
	{
		m_pMenu[i].Char.UnicodeChar = menu[i];
		m_pMenu[i].Attributes = static_cast<unsigned short>(ECHAR_COLOR::FG_WHITE);
	}
}

void SceneGameWin::Update(float _deltaSeconds)
{
	if (Input::IsKeyPressedThisFrame(VK_ESCAPE))
		Game::Get().SwitchScene(ESCENE_TYPE::MENU);
}

void SceneGameWin::Render()
{
	Renderer::Get().RenderAtPos(m_pGameWin, m_location, 8, 1);
	Renderer::Get().RenderAtPos(m_pMenu, m_startTextLocation, 18, 1);
}