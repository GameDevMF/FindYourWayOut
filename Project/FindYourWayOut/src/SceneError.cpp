#pragma region include project
#include "Input.hpp"
#include "Enums.hpp"
#include "Renderer.hpp"
#include "Game.hpp"
#include "SceneError.hpp"
#pragma endregion

SceneError::~SceneError()
{
	delete[] m_pError;
	delete[] m_pKey;
	delete[] m_pDetails;
}

void SceneError::Init()
{
	const char* error = "ERROR!!!";
	const char* key = "Press ESC to return to Menu!";

	m_pError = new CHAR_INFO[8];
	m_pKey = new CHAR_INFO[28];

	m_location.X = Renderer::Get().GetScreenWidth() / 2 - 4;
	m_location.Y = Renderer::Get().GetScreenHeight() / 2;

	m_keyTextPosition.X = Renderer::Get().GetScreenWidth() / 2 - 14;
	m_keyTextPosition.Y = Renderer::Get().GetScreenHeight() / 2 + 1;

	for (int i = 0; i < 8; i++)
	{
		m_pError[i].Char.UnicodeChar = error[i];
		m_pError[i].Attributes = ECHAR_COLOR::FG_RED;
	}

	for (int i = 0; i < 28; i++)
	{
		m_pKey[i].Char.UnicodeChar = key[i];
		m_pKey[i].Attributes = ECHAR_COLOR::FG_WHITE;
	}
}

void SceneError::Update(float _deltaSeconds)
{
	if (Input::IsKeyPressedThisFrame(VK_ESCAPE))
		Game::Get().SwitchScene(MENU);
}

void SceneError::Render()
{
	Renderer::Get().RenderAtPos(m_pError, m_location, 8, 1);
	Renderer::Get().RenderAtPos(m_pKey, m_keyTextPosition, 28, 1);

	if (m_pDetails && m_detailsTextLength)
		Renderer::Get().RenderAtPos(m_pDetails, m_detailsTextPosition, m_detailsTextLength, 1);
}

void SceneError::SetErrorDetails(const char* _pDetails)
{
	m_detailsTextLength = 0;

	for (int i = 0; i < Renderer::Get().GetScreenWidth(); i++)
	{
		if(_pDetails[i] == '\0')
			break;

		m_detailsTextLength++;
	}

	delete[] m_pDetails;
	m_pDetails = nullptr;

	if (!m_detailsTextLength)
		return;

	m_detailsTextPosition.X = Renderer::Get().GetScreenWidth() / 2 - (m_detailsTextLength * 0.5f);
	m_detailsTextPosition.Y = Renderer::Get().GetScreenHeight() / 2 + 3;

	m_pDetails = new CHAR_INFO[m_detailsTextLength];

	for (int i = 0; i < m_detailsTextLength; i++)
	{
		m_pDetails[i].Char.UnicodeChar = _pDetails[i];
		m_pDetails[i].Attributes = ECHAR_COLOR::FG_WHITE;
	}
}