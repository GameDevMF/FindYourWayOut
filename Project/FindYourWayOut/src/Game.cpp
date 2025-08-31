#pragma region include project
#include "Input.hpp"
#include "Renderer.hpp"
#include "SceneMenu.hpp"
#include "SceneLevel.hpp"
#include "SceneGameOver.hpp"
#include "SceneGameWin.hpp"
#include "SceneError.hpp"
#include "Game.hpp"
#pragma endregion

Game::~Game()
{
	delete m_pCurrentScene;
}

void Game::Init()
{
	Renderer::Get().Init();

	m_pCurrentScene = new SceneMenu();
	m_pCurrentScene->Init();
	
	m_isRunning = true;
}

void Game::SwitchScene(ESCENE_TYPE _type, int _level)
{
	if (_level > 99)
	{
		ShowErrorScene("Code 99: Max number of level is 99!");
	}
	else
	{
		m_isSceneChanging = true;
		m_currentSceneType = _type;
		m_level = _level;
	}
}

void Game::ShowErrorScene(std::string _pText)
{
	m_isSceneChanging = true;
	m_currentSceneType = ESCENE_TYPE::SHOW_ERROR;
	m_errorText = _pText;
}

void Game::Update()
{
	// calculate seconds since last frame
	m_deltaSeconds = static_cast<float>(NOW - m_timeLastUpdate) / 1000.0f;
	m_timeLastUpdate = NOW;

	Input::UpdateBeginFrame();

	if(!m_isSceneChanging)
		m_pCurrentScene->Update(m_deltaSeconds);
}

void Game::Render()
{
	if (m_isSceneChanging)
		ChangeScene();

	m_pCurrentScene->Render();

	Renderer::Get().Render();
}

void Game::ChangeScene()
{
	delete m_pCurrentScene;
	m_pCurrentScene = nullptr;

	m_isSceneChanging = false;

	switch (m_currentSceneType)
	{
	case ESCENE_TYPE::MENU:
		m_pCurrentScene = new SceneMenu();
		break;
	case ESCENE_TYPE::LEVEL:
		m_pCurrentScene = new SceneLevel();
		break;
	case ESCENE_TYPE::GAME_OVER:
		m_pCurrentScene = new SceneGameOver();
		break;
	case ESCENE_TYPE::GAME_WIN:
		m_pCurrentScene = new SceneGameWin();
		break;
	case ESCENE_TYPE::SHOW_ERROR:
	{
		m_pCurrentScene = new SceneError();
		static_cast<SceneError*>(m_pCurrentScene)->SetErrorDetails(m_errorText.c_str());
		break;
	}
	default:
		break;
	}
	
	m_pCurrentScene->Init();
}