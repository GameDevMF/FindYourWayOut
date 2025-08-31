#pragma region include system
#include <sstream>
#include <fstream>
#include <string>
#pragma endregion

#pragma region include project
#include "Game.hpp"
#include "Wall.hpp"
#include "Camera.hpp"
#include "Minimap.hpp"
#include "HUD.hpp"
#include "Raytracer.hpp"
#include "SceneError.hpp"
#include "ObjectManager.hpp"
#pragma endregion

void ObjectManager::Update(float _deltaSeconds)
{
	m_pPlayer->Update(_deltaSeconds);

	// update objects below x and below y player chunk
	if (m_playerChunk.X > 0.0f && m_playerChunk.Y > 0.0f)
		for (Object* pObject : m_pDynamicObjectChunks[static_cast<int>(((m_playerChunk.Y - 1.0f) * m_levelChunkCount.X) + (m_playerChunk.X - 1.0f))])
			pObject->Update(_deltaSeconds);

	// update objects x and below y player chunk
	if (m_playerChunk.Y > 0.0f)
		for (Object* pObject : m_pDynamicObjectChunks[static_cast<int>(((m_playerChunk.Y - 1.0f) * m_levelChunkCount.X) + m_playerChunk.X)])
			pObject->Update(_deltaSeconds);

	// update objects above x and below y player chunk
	if (m_playerChunk.X < m_levelChunkCount.X - 1.0f && m_playerChunk.Y > 0.0f)
		for (Object* pObject : m_pDynamicObjectChunks[static_cast<int>(((m_playerChunk.Y - 1.0f) * m_levelChunkCount.X) + (m_playerChunk.X + 1.0f))])
			pObject->Update(_deltaSeconds);

	// update objects below x and y player chunk
	if (m_playerChunk.X > 0.0f)
		for (Object* pObject : m_pDynamicObjectChunks[static_cast<int>((m_playerChunk.Y * m_levelChunkCount.X) + (m_playerChunk.X - 1.0f))])
			pObject->Update(_deltaSeconds);

	// update objects where the player location is
	for (Object* pObject : m_pDynamicObjectChunks[static_cast<int>((m_playerChunk.Y * m_levelChunkCount.X) + m_playerChunk.X)])
		pObject->Update(_deltaSeconds);

	// update objects above x and y player chunk
	if (m_playerChunk.X < m_levelChunkCount.X - 1.0f)
		for (Object* pObject : m_pDynamicObjectChunks[static_cast<int>((m_playerChunk.Y * m_levelChunkCount.X) + (m_playerChunk.X + 1.0f))])
			pObject->Update(_deltaSeconds);

	// update objects below x and above y player chunk
	if (m_playerChunk.X > 0.0f && m_playerChunk.Y < m_levelChunkCount.Y - 1.0f)
		for (Object* pObject : m_pDynamicObjectChunks[static_cast<int>(((m_playerChunk.Y + 1.0f) * m_levelChunkCount.X) + (m_playerChunk.X - 1.0f))])
			pObject->Update(_deltaSeconds);

	// update objects x and above y player chunk
	if (m_playerChunk.Y < m_levelChunkCount.Y - 1.0f)
		for (Object* pObject : m_pDynamicObjectChunks[static_cast<int>(((m_playerChunk.Y + 1.0f) * m_levelChunkCount.X) + m_playerChunk.X)])
			pObject->Update(_deltaSeconds);

	// update objects above x and above y player chunk
	if (m_playerChunk.X < m_levelChunkCount.X - 1.0f && m_playerChunk.Y < m_levelChunkCount.Y - 1.0f)
		for (Object* pObject : m_pDynamicObjectChunks[static_cast<int>(((m_playerChunk.Y + 1.0f) * m_levelChunkCount.X) + (m_playerChunk.X + 1.0f))])
			pObject->Update(_deltaSeconds);

	// player moved to chunk below x and below y
	if (m_pPlayer->GetLocation().X < m_playerChunk.X * m_levelConfig.CameraMaxViewDistance && m_pPlayer->GetLocation().Y < m_playerChunk.Y * m_levelConfig.CameraMaxViewDistance)
	{
		ChangePlayerChunk(m_playerChunk.X - 1, m_playerChunk.Y - 1);
	}

	// player moved to chunk below y
	else if (m_pPlayer->GetLocation().X >= m_playerChunk.X * m_levelConfig.CameraMaxViewDistance &&
		m_pPlayer->GetLocation().X < m_playerChunk.X * m_levelConfig.CameraMaxViewDistance + m_levelConfig.CameraMaxViewDistance &&
		m_pPlayer->GetLocation().Y < m_playerChunk.Y * m_levelConfig.CameraMaxViewDistance)
	{
		ChangePlayerChunk(m_playerChunk.X, m_playerChunk.Y - 1);
	}

	// player moved to chunk above x and below y
	else if (m_pPlayer->GetLocation().X >= m_playerChunk.X * m_levelConfig.CameraMaxViewDistance + m_levelConfig.CameraMaxViewDistance &&
		m_pPlayer->GetLocation().Y < m_playerChunk.Y * m_levelConfig.CameraMaxViewDistance)
	{
		ChangePlayerChunk(m_playerChunk.X + 1, m_playerChunk.Y - 1);
	}

	// player moved to chunk below x
	else if (m_pPlayer->GetLocation().X < m_playerChunk.X * m_levelConfig.CameraMaxViewDistance &&
		m_pPlayer->GetLocation().Y >= m_playerChunk.Y * m_levelConfig.CameraMaxViewDistance &&
		m_pPlayer->GetLocation().Y < m_playerChunk.Y * m_levelConfig.CameraMaxViewDistance + m_levelConfig.CameraMaxViewDistance)
	{
		ChangePlayerChunk(m_playerChunk.X - 1, m_playerChunk.Y);
	}

	// player moved to chunk above x
	else if (m_pPlayer->GetLocation().X >= m_playerChunk.X * m_levelConfig.CameraMaxViewDistance + m_levelConfig.CameraMaxViewDistance &&
		m_pPlayer->GetLocation().Y >= m_playerChunk.Y * m_levelConfig.CameraMaxViewDistance &&
		m_pPlayer->GetLocation().Y < m_playerChunk.Y * m_levelConfig.CameraMaxViewDistance + m_levelConfig.CameraMaxViewDistance)
	{
		ChangePlayerChunk(m_playerChunk.X + 1, m_playerChunk.Y);
	}

	// player moved to chunk below x and above y
	else if (m_pPlayer->GetLocation().X < m_playerChunk.X * m_levelConfig.CameraMaxViewDistance &&
		m_pPlayer->GetLocation().Y >= m_playerChunk.Y * m_levelConfig.CameraMaxViewDistance + m_levelConfig.CameraMaxViewDistance)
	{
		ChangePlayerChunk(m_playerChunk.X - 1, m_playerChunk.Y + 1);
	}

	// player moved to chunk above y
	else if (m_pPlayer->GetLocation().X >= m_playerChunk.X * m_levelConfig.CameraMaxViewDistance &&
		m_pPlayer->GetLocation().X < m_playerChunk.X * m_levelConfig.CameraMaxViewDistance + m_levelConfig.CameraMaxViewDistance &&
		m_pPlayer->GetLocation().Y >= m_playerChunk.Y * m_levelConfig.CameraMaxViewDistance + m_levelConfig.CameraMaxViewDistance)
	{
		ChangePlayerChunk(m_playerChunk.X, m_playerChunk.Y + 1);
	}

	// player moved to chunk above x and above y
	else if (m_pPlayer->GetLocation().X >= m_playerChunk.X * m_levelConfig.CameraMaxViewDistance + m_levelConfig.CameraMaxViewDistance &&
		m_pPlayer->GetLocation().Y >= m_playerChunk.Y * m_levelConfig.CameraMaxViewDistance + m_levelConfig.CameraMaxViewDistance)
	{
		ChangePlayerChunk(m_playerChunk.X + 1, m_playerChunk.Y + 1);
	}

	// loop until no objects to remove left
	while (!m_objectsToRemove.empty())
	{
		Object* pObject = m_objectsToRemove.front();

		m_pStaticObjects.remove(pObject);
		m_pDynamicObjects.remove(pObject);

		for (int y = 0; y < m_levelChunkCount.Y; y++)
		{
			for (int x = 0; x < m_levelChunkCount.X; x++)
			{
				m_pStaticObjectChunks[y * static_cast<int>(m_levelChunkCount.X) + x].remove(pObject);
				m_pDynamicObjectChunks[y * static_cast<int>(m_levelChunkCount.X) + x].remove(pObject);
			}
		}

		m_objectsToRemove.remove(pObject);
		delete pObject;
	}
}

void ObjectManager::Render()
{
	if (m_pPlayer)
		m_pPlayer->Render();
}

void ObjectManager::LoadLevel()
{
	ResetLevelConfig();
	LoadLevelConfigFromFile();
	LoadLevelMapFromFile();
}

void ObjectManager::ClearLevel(bool _keepPlayer)
{
	// delete chunks arrays but not the objects in the lists
	delete[] m_pStaticObjectChunks;
	m_pStaticObjectChunks = nullptr;
	delete[] m_pDynamicObjectChunks;
	m_pDynamicObjectChunks = nullptr;

	// delete every static object
	if (m_pStaticObjects.size())
	{
		for (Object* pObject : m_pStaticObjects)
			delete pObject;
		m_pStaticObjects.clear();
	}

	// delete every dynamic object
	if (m_pDynamicObjects.size())
	{
		for (Object* pObject : m_pDynamicObjects)
			delete pObject;
		m_pDynamicObjects.clear();
	}

	if(_keepPlayer || !m_pPlayer)
		return;
	
	delete m_pPlayer;
	m_pPlayer = nullptr;
}

void ObjectManager::RemoveObject(Object* _pObject)
{
	m_objectsToRemove.push_back(_pObject);
}

bool ObjectManager::HitWall(SVector2 _location)
{
	for (int y = 0; y < m_levelChunkCount.Y; y++)
	{
		for (int x = 0; x < m_levelChunkCount.X; x++)
		{
			// if object is in current chunk
			if ((_location.X >= x * m_levelConfig.CameraMaxViewDistance &&
				_location.X < x * m_levelConfig.CameraMaxViewDistance + m_levelConfig.CameraMaxViewDistance)
				&&
				(_location.Y >= y * m_levelConfig.CameraMaxViewDistance &&
					_location.Y < y * m_levelConfig.CameraMaxViewDistance + m_levelConfig.CameraMaxViewDistance))
			{
				// set hit chunk
				m_hitChunk.X = x;
				m_hitChunk.Y = y;
			}
		}
	}

	// check objects in chunk below x and below y of hit chunk for collision with wall
	if (m_hitChunk.X > 0.0f && m_hitChunk.Y > 0.0f)
	{
		for (Object* pObject : m_pStaticObjectChunks[static_cast<int>(((m_hitChunk.Y - 1.0f) * m_levelChunkCount.X) + (m_hitChunk.X - 1.0f))])
		{
			if (dynamic_cast<Wall*>(pObject))
			{
				// if location is inside the wall return collision true
				if (_location.X >= pObject->GetLocation().X && _location.X <= pObject->GetLocation().X + m_levelConfig.TileSizeCm &&
					_location.Y >= pObject->GetLocation().Y && _location.Y <= pObject->GetLocation().Y + m_levelConfig.TileSizeCm)
					return true;
			}
		}
	}

	// check objects in chunk x and below y of hit chunk for collision with wall
	if (m_hitChunk.Y > 0.0f)
	{
		for (Object* pObject : m_pStaticObjectChunks[static_cast<int>(((m_hitChunk.Y - 1.0f) * m_levelChunkCount.X) + m_hitChunk.X)])
		{
			if (dynamic_cast<Wall*>(pObject))
			{
				// if location is inside the wall return collision true
				if (_location.X >= pObject->GetLocation().X && _location.X <= pObject->GetLocation().X + m_levelConfig.TileSizeCm &&
					_location.Y >= pObject->GetLocation().Y && _location.Y <= pObject->GetLocation().Y + m_levelConfig.TileSizeCm)
					return true;
			}
		}
	}

	// check objects in chunk above x and below y of hit chunk for collision with wall
	if (m_hitChunk.X < m_levelChunkCount.X - 1.0f && m_hitChunk.Y > 0.0f)
	{
		for (Object* pObject : m_pStaticObjectChunks[static_cast<int>(((m_hitChunk.Y - 1.0f) * m_levelChunkCount.X) + (m_hitChunk.X + 1.0f))])
		{
			if (dynamic_cast<Wall*>(pObject))
			{
				// if location is inside the wall return collision true
				if (_location.X >= pObject->GetLocation().X && _location.X <= pObject->GetLocation().X + m_levelConfig.TileSizeCm &&
					_location.Y >= pObject->GetLocation().Y && _location.Y <= pObject->GetLocation().Y + m_levelConfig.TileSizeCm)
					return true;
			}
		}
	}

	// check objects in chunk below x and y of hit chunk for collision with wall
	if (m_hitChunk.X > 0.0f)
	{
		for (Object* pObject : m_pStaticObjectChunks[static_cast<int>((m_hitChunk.Y * m_levelChunkCount.X) + (m_hitChunk.X - 1.0f))])
		{
			if (dynamic_cast<Wall*>(pObject))
			{
				// if location is inside the wall return collision true
				if (_location.X >= pObject->GetLocation().X && _location.X <= pObject->GetLocation().X + m_levelConfig.TileSizeCm &&
					_location.Y >= pObject->GetLocation().Y && _location.Y <= pObject->GetLocation().Y + m_levelConfig.TileSizeCm)
					return true;
			}
		}
	}

	// check objects in chunk where the location is of hit chunk for collision with wall
	for (Object* pObject : m_pStaticObjectChunks[static_cast<int>((m_hitChunk.Y * m_levelChunkCount.X) + m_hitChunk.X)])
	{
		if (dynamic_cast<Wall*>(pObject))
		{
			// if location is inside the wall return collision true
			if (_location.X >= pObject->GetLocation().X && _location.X <= pObject->GetLocation().X + m_levelConfig.TileSizeCm &&
				_location.Y >= pObject->GetLocation().Y && _location.Y <= pObject->GetLocation().Y + m_levelConfig.TileSizeCm)
				return true;
		}
	}

	// check objects in chunk above x and y of hit chunk for collision with wall
	if (m_hitChunk.X < m_levelChunkCount.X - 1.0f)
	{
		for (Object* pObject : m_pStaticObjectChunks[static_cast<int>((m_hitChunk.Y * m_levelChunkCount.X) + (m_hitChunk.X + 1.0f))])
		{
			if (dynamic_cast<Wall*>(pObject))
			{
				// if location is inside the wall return collision true
				if (_location.X >= pObject->GetLocation().X && _location.X <= pObject->GetLocation().X + m_levelConfig.TileSizeCm &&
					_location.Y >= pObject->GetLocation().Y && _location.Y <= pObject->GetLocation().Y + m_levelConfig.TileSizeCm)
					return true;
			}
		}
	}

	// check objects in chunk below x and above y of hit chunk for collision with wall
	if (m_hitChunk.X > 0.0f && m_hitChunk.Y < m_levelChunkCount.Y - 1.0f)
	{
		for (Object* pObject : m_pStaticObjectChunks[static_cast<int>(((m_hitChunk.Y + 1.0f) * m_levelChunkCount.X) + (m_hitChunk.X - 1.0f))])
		{
			if (dynamic_cast<Wall*>(pObject))
			{
				// if location is inside the wall return collision true
				if (_location.X >= pObject->GetLocation().X && _location.X <= pObject->GetLocation().X + m_levelConfig.TileSizeCm &&
					_location.Y >= pObject->GetLocation().Y && _location.Y <= pObject->GetLocation().Y + m_levelConfig.TileSizeCm)
					return true;
			}
		}
	}

	// check objects in chunk x and above y of hit chunk for collision with wall
	if (m_hitChunk.Y < m_levelChunkCount.Y - 1.0f)
	{
		for (Object* pObject : m_pStaticObjectChunks[static_cast<int>(((m_hitChunk.Y + 1.0f) * m_levelChunkCount.X) + m_hitChunk.X)])
		{
			if (dynamic_cast<Wall*>(pObject))
			{
				// if location is inside the wall return collision true
				if (_location.X >= pObject->GetLocation().X && _location.X <= pObject->GetLocation().X + m_levelConfig.TileSizeCm &&
					_location.Y >= pObject->GetLocation().Y && _location.Y <= pObject->GetLocation().Y + m_levelConfig.TileSizeCm)
					return true;
			}
		}
	}

	// check objects in chunk above x and above y of hit chunk for collision with wall
	if (m_hitChunk.X < m_levelChunkCount.X - 1.0f && m_hitChunk.Y < m_levelChunkCount.Y - 1.0f)
	{
		for (Object* pObject : m_pStaticObjectChunks[static_cast<int>(((m_hitChunk.Y + 1.0f) * m_levelChunkCount.X) + (m_hitChunk.X + 1.0f))])
		{
			if (dynamic_cast<Wall*>(pObject))
			{
				// if location is inside the wall return collision true
				if (_location.X >= pObject->GetLocation().X && _location.X <= pObject->GetLocation().X + m_levelConfig.TileSizeCm &&
					_location.Y >= pObject->GetLocation().Y && _location.Y <= pObject->GetLocation().Y + m_levelConfig.TileSizeCm)
					return true;
			}
		}
	}

	// if no wall was hit return false
	return false;
}

void ObjectManager::ResetLevelConfig()
{
	m_levelConfig.CameraFOV = SVector2(90.0f, 90.0f);
	m_levelConfig.CameraMaxViewDistance = 500;
	m_levelConfig.TileSizeCm = 0;
	m_levelConfig.LastLevel = false;
	m_levelConfig.Time = 240;
	m_levelConfig.WallChar = ' ';
	m_levelConfig.PlayerChar = ' ';
	m_levelConfig.ExitChar = ' ';
}

void ObjectManager::LoadLevelConfigFromFile()
{
	m_isLevelConfigLoadError = false;

	// get level file from game level
	std::string file = "Assets/level_";
	file += std::to_string(Game::Get().GetLevel());
	file += ".cfg";

	std::ifstream infile(file);

	// if file could not open show error scene and return
	if (!infile.is_open())
	{
		std::string errorText = "Code 10: File ";
		errorText += file;
		errorText += " failed to open!";
		
		Game::Get().ShowErrorScene(errorText);
		m_isLevelConfigLoadError = true;
		return;
	}

	std::string line;
	std::string key;
	std::string value;

	while (std::getline(infile, line))
	{
		// if no separator in line show error scene and return
		if (!line.find(':'))
		{
			std::string errorText = "Code 12: Separator : is missing in ";
			errorText += file;
			errorText += ", Line: '";
			errorText += line;
			errorText += "'";

			Game::Get().ShowErrorScene(errorText);
			m_isLevelConfigLoadError = true;
			return;
		}

		key = line.substr(0, line.find(':'));
		value = line.substr(line.find(':') + 1, line.size() - 1);

		// if key or value empty show error scene and return
		if (!key.size() || !value.size())
		{
			std::string errorText = "Code 13: Key or Value missing in ";
			errorText += file;
			errorText += ", Line: '";
			errorText += line;
			errorText += "'";

			Game::Get().ShowErrorScene(errorText);
			m_isLevelConfigLoadError = true;
			return;
		}

		if (key == "camera_fov_horizontal")
			m_levelConfig.CameraFOV.X = std::atoi(value.c_str());
		else if (key == "camera_fov_vertical")
			m_levelConfig.CameraFOV.Y = std::atoi(value.c_str());
		else if (key == "camera_max_view_distance")
			m_levelConfig.CameraMaxViewDistance = std::atoi(value.c_str());
		else if(key == "tile_size_cm")
			m_levelConfig.TileSizeCm = std::atoi(value.c_str());
		else if(key == "last_level")
			m_levelConfig.LastLevel = std::atoi(value.c_str());
		else if (key == "time")
			m_levelConfig.Time = std::atoi(value.c_str());
		else if(key == "wall_char")
			m_levelConfig.WallChar = value[0];
		else if(key == "player_char")
			m_levelConfig.PlayerChar = value[0];
		else if (key == "exit_char")
			m_levelConfig.ExitChar = value[0];
	}

	// if no wall char set show error scene and return
	if (m_levelConfig.WallChar == 32)
	{
		std::string errorText = "Code 14: No wall char set in File '";
		errorText += file;
		errorText += "'";

		Game::Get().ShowErrorScene(errorText);
		m_isLevelConfigLoadError = true;
		return;
	}

	// if no exit char set show error scene and return
	if (m_levelConfig.ExitChar == 32)
	{
		std::string errorText = "Code 15: No exit char set in File '";
		errorText += file;
		errorText += "'";

		Game::Get().ShowErrorScene(errorText);
		m_isLevelConfigLoadError = true;
		return;
	}

	// if no player char set show error scene and return
	if (m_levelConfig.PlayerChar == 32)
	{
		std::string errorText = "Code 16: No player char set in File '";
		errorText += file;
		errorText += "'";

		Game::Get().ShowErrorScene(errorText);
		m_isLevelConfigLoadError = true;
		return;
	}
}

void ObjectManager::LoadLevelMapFromFile()
{
	if (m_isLevelConfigLoadError)
		return;

	// get level file from game level
	std::string file = "Assets/level_";
	file += std::to_string(Game::Get().GetLevel());
	file += ".map";

	std::ifstream infile(file);

	// if file could not open show error scene and return
	if (!infile.is_open())
	{
		std::string errorText = "Code 17: File ";
		errorText += file;
		errorText += " failed to open!";

		Game::Get().ShowErrorScene(errorText);
		return;
	}

	std::string line;

	m_levelChunkCount.X = 0;
	m_levelChunkCount.Y = 0;

	SVector2 exitLocation(-1.0f, -1.0f);

	while (std::getline(infile, line))
	{
		int levelSizeX = 0;

		for (int i = 0; i < line.size(); i++)
		{
			levelSizeX++;

			if(line[i] == ' ')
				continue;

			if (line[i] == m_levelConfig.WallChar)
			{
				Object* pWall = new Wall({ static_cast<float>(i) * m_levelConfig.TileSizeCm, m_levelChunkCount.Y * m_levelConfig.TileSizeCm });
				m_pStaticObjects.push_back(pWall);
			}
			else if (line[i] == m_levelConfig.PlayerChar)
			{
				if (!m_pPlayer)
					m_pPlayer = new Player({ i * m_levelConfig.TileSizeCm + m_levelConfig.TileSizeCm * 0.5f,
						m_levelChunkCount.Y * m_levelConfig.TileSizeCm + m_levelConfig.TileSizeCm * 0.5f });
				else
					m_pPlayer->SetLocation({ i * m_levelConfig.TileSizeCm + m_levelConfig.TileSizeCm * 0.5f,
						m_levelChunkCount.Y * m_levelConfig.TileSizeCm + m_levelConfig.TileSizeCm * 0.5f });

				m_pPlayer->Init();
			}
			else if (line[i] == m_levelConfig.ExitChar)
			{
				exitLocation.X = i * m_levelConfig.TileSizeCm + (m_levelConfig.TileSizeCm * 0.5f);
				exitLocation.Y = m_levelChunkCount.Y * m_levelConfig.TileSizeCm + (m_levelConfig.TileSizeCm * 0.5f);

				Raytracer::Get().SetExitLocation(exitLocation);
			}
		}

		if (levelSizeX > m_levelChunkCount.X)
			m_levelChunkCount.X = levelSizeX;

		m_levelChunkCount.Y++;
	}

	// if no player
	if (!m_pPlayer)
	{
		std::string errorText = "Code 18: No Player was create from file: '";
		errorText += file;
		errorText += "'";

		Game::Get().ShowErrorScene(errorText);
		return;
	}

	// if exit location below level show error scene and return
	if (exitLocation.X <= 0.0f || exitLocation.Y <= 0.0f)
	{
		std::string errorText = "Code 19: Exit Location is below level in map: '";
		errorText += file;
		errorText += "'";

		Game::Get().ShowErrorScene(errorText);
		return;
	}

	m_pPlayer->SetExitLocation(exitLocation);

	// calculate size of one chunk
	int tilesPerChunk = static_cast<int>(m_levelConfig.CameraMaxViewDistance / m_levelConfig.TileSizeCm);

	// calculate level chunk count
	m_levelChunkCount.X = static_cast<int>(m_levelChunkCount.X / tilesPerChunk + 1.0f);
	m_levelChunkCount.Y = static_cast<int>(m_levelChunkCount.Y / tilesPerChunk + 1.0f);

	m_pStaticObjectChunks = new std::list<Object*>[m_levelChunkCount.X * m_levelChunkCount.Y];
	m_pDynamicObjectChunks = new std::list<Object*>[m_levelChunkCount.X * m_levelChunkCount.Y];
	SplitObjectsInChunks();

	Raytracer::Get().Prepare();
}

void ObjectManager::SplitObjectsInChunks()
{
	for (int y = 0; y < m_levelChunkCount.Y; y++)
	{
		for (int x = 0; x < m_levelChunkCount.X; x++)
		{
			for (Object* pObject : m_pStaticObjects)
			{
				if (dynamic_cast<Wall*>(pObject))
				{
					// if wall is in current chunk add it to it
					if (((pObject->GetLocation().X > x * m_levelConfig.CameraMaxViewDistance &&
						pObject->GetLocation().X < x * m_levelConfig.CameraMaxViewDistance + m_levelConfig.CameraMaxViewDistance) ||
						(pObject->GetLocation().X + m_levelConfig.TileSizeCm > x * m_levelConfig.CameraMaxViewDistance &&
							pObject->GetLocation().X + m_levelConfig.TileSizeCm < x * m_levelConfig.CameraMaxViewDistance + m_levelConfig.CameraMaxViewDistance))
						&&
						((pObject->GetLocation().Y > y * m_levelConfig.CameraMaxViewDistance &&
							pObject->GetLocation().Y < y * m_levelConfig.CameraMaxViewDistance + m_levelConfig.CameraMaxViewDistance) ||
							(pObject->GetLocation().Y + m_levelConfig.TileSizeCm > y * m_levelConfig.CameraMaxViewDistance &&
								pObject->GetLocation().Y + m_levelConfig.TileSizeCm < y * m_levelConfig.CameraMaxViewDistance + m_levelConfig.CameraMaxViewDistance)))
					{
						m_pStaticObjectChunks[y * static_cast<int>(m_levelChunkCount.X) + x].push_back(pObject);
					}
				}
				else
				{
					// if object is in current chunk add it to it
					if ((pObject->GetLocation().X >= x * m_levelConfig.CameraMaxViewDistance &&
						pObject->GetLocation().X < x * m_levelConfig.CameraMaxViewDistance + m_levelConfig.CameraMaxViewDistance)
						&&
						(pObject->GetLocation().Y >= y * m_levelConfig.CameraMaxViewDistance &&
							pObject->GetLocation().Y < y * m_levelConfig.CameraMaxViewDistance + m_levelConfig.CameraMaxViewDistance))
					{
						m_pStaticObjectChunks[y * static_cast<int>(m_levelChunkCount.X) + x].push_back(pObject);
					}
				}
			}

			for (Object* pObject : m_pDynamicObjects)
			{
				// if object is in current chunk add it to it
				if ((pObject->GetLocation().X >= x * m_levelConfig.CameraMaxViewDistance &&
					pObject->GetLocation().X < x * m_levelConfig.CameraMaxViewDistance + m_levelConfig.CameraMaxViewDistance)
					&&
					(pObject->GetLocation().Y >= y * m_levelConfig.CameraMaxViewDistance &&
						pObject->GetLocation().Y < y * m_levelConfig.CameraMaxViewDistance + m_levelConfig.CameraMaxViewDistance))
				{
					m_pDynamicObjectChunks[y * static_cast<int>(m_levelChunkCount.X) + x].push_back(pObject);

					// if current dynamic object is the player set current player chunk
					if (dynamic_cast<Player*>(pObject))
					{
						m_playerChunk.X = x;
						m_playerChunk.Y = y;
					}
				}
			}
		}
	}
}

void ObjectManager::ChangePlayerChunk(int _playerChunkX, int _playerChunkY)
{
	// remove player from current chunk
	m_pDynamicObjectChunks[static_cast<int>(m_playerChunk.Y * m_levelChunkCount.X + m_playerChunk.X)].remove(m_pPlayer);

	// change chunk
	m_playerChunk.X = _playerChunkX;
	m_playerChunk.Y = _playerChunkY;

	// add player to new chunk
	m_pDynamicObjectChunks[static_cast<int>(m_playerChunk.Y * m_levelChunkCount.X + m_playerChunk.X)].push_back(m_pPlayer);
}