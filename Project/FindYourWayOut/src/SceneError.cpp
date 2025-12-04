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
	const char* error{ "ERROR!!!" };
	const char* key{ "Press ESC to return to Menu!" };
	const float screenWidthHalf{ Renderer::Get().GetScreenWidth() * 0.5f };
	const float screenHeightHalf{ Renderer::Get().GetScreenHeight() * 0.5f };

	m_pError = new CHAR_INFO[8];
	m_pKey = new CHAR_INFO[28];

	m_location = { screenWidthHalf - 4.0f, screenHeightHalf };

	m_keyTextPosition = { screenWidthHalf - 14.0f, screenHeightHalf + 1.0f };

	for (int i{ 0 }; i < 8; i++)
	{
		m_pError[i].Char.UnicodeChar = error[i];
		m_pError[i].Attributes = static_cast<unsigned short>(ECHAR_COLOR::FG_RED);
	}

	for (int i{ 0 }; i < 28; i++)
	{
		m_pKey[i].Char.UnicodeChar = key[i];
		m_pKey[i].Attributes = static_cast<unsigned short>(ECHAR_COLOR::FG_WHITE);
	}
}

void SceneError::Update(float _deltaSeconds)
{
	if (Input::IsKeyPressedThisFrame(VK_ESCAPE))
		Game::Get().SwitchScene(ESCENE_TYPE::MENU);
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

	for (int i{ 0 }; i < Renderer::Get().GetScreenWidth(); i++)
	{
		if(_pDetails[i] == '\0')
			break;

		m_detailsTextLength++;
	}

	delete[] m_pDetails;
	m_pDetails = nullptr;

	if (!m_detailsTextLength)
		return;

	m_detailsTextPosition = { Renderer::Get().GetScreenWidth() * 0.5f - (m_detailsTextLength * 0.5f), Renderer::Get().GetScreenHeight() * 0.5f + 3.0f };

	m_pDetails = new CHAR_INFO[m_detailsTextLength];

	for (int i{ 0 }; i < m_detailsTextLength; i++)
	{
		m_pDetails[i].Char.UnicodeChar = _pDetails[i];
		m_pDetails[i].Attributes = static_cast<unsigned short>(ECHAR_COLOR::FG_WHITE);
	}
}