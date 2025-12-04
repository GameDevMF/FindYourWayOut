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
		for (const std::shared_ptr<Object>& pObject : m_pDynamicObjectChunks[static_cast<int>(((m_playerChunk.Y - 1.0f) * m_levelChunkCount.X) + (m_playerChunk.X - 1.0f))])
			pObject->Update(_deltaSeconds);

	// update objects x and below y player chunk
	if (m_playerChunk.Y > 0.0f)
		for (const std::shared_ptr<Object>& pObject : m_pDynamicObjectChunks[static_cast<int>(((m_playerChunk.Y - 1.0f) * m_levelChunkCount.X) + m_playerChunk.X)])
			pObject->Update(_deltaSeconds);

	// update objects above x and below y player chunk
	if (m_playerChunk.X < m_levelChunkCount.X - 1.0f && m_playerChunk.Y > 0.0f)
		for (const std::shared_ptr<Object>& pObject : m_pDynamicObjectChunks[static_cast<int>(((m_playerChunk.Y - 1.0f) * m_levelChunkCount.X) + (m_playerChunk.X + 1.0f))])
			pObject->Update(_deltaSeconds);

	// update objects below x and y player chunk
	if (m_playerChunk.X > 0.0f)
		for (const std::shared_ptr<Object>& pObject : m_pDynamicObjectChunks[static_cast<int>((m_playerChunk.Y * m_levelChunkCount.X) + (m_playerChunk.X - 1.0f))])
			pObject->Update(_deltaSeconds);

	// update objects where the player location is
	for (const std::shared_ptr<Object>& pObject : m_pDynamicObjectChunks[static_cast<int>((m_playerChunk.Y * m_levelChunkCount.X) + m_playerChunk.X)])
		pObject->Update(_deltaSeconds);

	// update objects above x and y player chunk
	if (m_playerChunk.X < m_levelChunkCount.X - 1.0f)
		for (const std::shared_ptr<Object>& pObject : m_pDynamicObjectChunks[static_cast<int>((m_playerChunk.Y * m_levelChunkCount.X) + (m_playerChunk.X + 1.0f))])
			pObject->Update(_deltaSeconds);

	// update objects below x and above y player chunk
	if (m_playerChunk.X > 0.0f && m_playerChunk.Y < m_levelChunkCount.Y - 1.0f)
		for (const std::shared_ptr<Object>& pObject : m_pDynamicObjectChunks[static_cast<int>(((m_playerChunk.Y + 1.0f) * m_levelChunkCount.X) + (m_playerChunk.X - 1.0f))])
			pObject->Update(_deltaSeconds);

	// update objects x and above y player chunk
	if (m_playerChunk.Y < m_levelChunkCount.Y - 1.0f)
		for (const std::shared_ptr<Object>& pObject : m_pDynamicObjectChunks[static_cast<int>(((m_playerChunk.Y + 1.0f) * m_levelChunkCount.X) + m_playerChunk.X)])
			pObject->Update(_deltaSeconds);

	// update objects above x and above y player chunk
	if (m_playerChunk.X < m_levelChunkCount.X - 1.0f && m_playerChunk.Y < m_levelChunkCount.Y - 1.0f)
		for (const std::shared_ptr<Object>& pObject : m_pDynamicObjectChunks[static_cast<int>(((m_playerChunk.Y + 1.0f) * m_levelChunkCount.X) + (m_playerChunk.X + 1.0f))])
			pObject->Update(_deltaSeconds);

	const SVector2 playerLocation{ m_pPlayer->GetLocation() };
	const float chunkX{ m_playerChunk.X * m_levelConfig.CameraMaxViewDistance };
	const float chunkY{ m_playerChunk.Y * m_levelConfig.CameraMaxViewDistance };

	// player moved to chunk below x and below y
	if (playerLocation.X < chunkX && playerLocation.Y < chunkY)
	{
		ChangePlayerChunk(m_playerChunk.X - 1, m_playerChunk.Y - 1);
	}

	// player moved to chunk below y
	else if (playerLocation.X >= chunkX &&
		playerLocation.X < chunkX + m_levelConfig.CameraMaxViewDistance &&
		playerLocation.Y < chunkY)
	{
		ChangePlayerChunk(m_playerChunk.X, m_playerChunk.Y - 1);
	}

	// player moved to chunk above x and below y
	else if (playerLocation.X >= chunkX + m_levelConfig.CameraMaxViewDistance &&
		playerLocation.Y < chunkY)
	{
		ChangePlayerChunk(m_playerChunk.X + 1, m_playerChunk.Y - 1);
	}

	// player moved to chunk below x
	else if (playerLocation.X < chunkX &&
		playerLocation.Y >= chunkY &&
		playerLocation.Y < chunkY + m_levelConfig.CameraMaxViewDistance)
	{
		ChangePlayerChunk(m_playerChunk.X - 1, m_playerChunk.Y);
	}

	// player moved to chunk above x
	else if (playerLocation.X >= chunkX + m_levelConfig.CameraMaxViewDistance &&
		playerLocation.Y >= chunkY &&
		playerLocation.Y < chunkY + m_levelConfig.CameraMaxViewDistance)
	{
		ChangePlayerChunk(m_playerChunk.X + 1, m_playerChunk.Y);
	}

	// player moved to chunk below x and above y
	else if (playerLocation.X < chunkX &&
		playerLocation.Y >= chunkY + m_levelConfig.CameraMaxViewDistance)
	{
		ChangePlayerChunk(m_playerChunk.X - 1, m_playerChunk.Y + 1);
	}

	// player moved to chunk above y
	else if (playerLocation.X >= chunkX &&
		playerLocation.X < chunkX + m_levelConfig.CameraMaxViewDistance &&
		playerLocation.Y >= chunkY + m_levelConfig.CameraMaxViewDistance)
	{
		ChangePlayerChunk(m_playerChunk.X, m_playerChunk.Y + 1);
	}

	// player moved to chunk above x and above y
	else if (playerLocation.X >= chunkX + m_levelConfig.CameraMaxViewDistance &&
		playerLocation.Y >= chunkY + m_levelConfig.CameraMaxViewDistance)
	{
		ChangePlayerChunk(m_playerChunk.X + 1, m_playerChunk.Y + 1);
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
	if (m_staticObjects.size())
		m_staticObjects.clear();

	// delete every dynamic object
	if (m_dynamicObjects.size())
		m_dynamicObjects.clear();
}

bool ObjectManager::HitWall(SVector2 _location)
{
	for (int y{ 0 }; y < m_levelChunkCount.Y; y++)
	{
		for (int x{ 0 }; x < m_levelChunkCount.X; x++)
		{
			const float chunkMinX{ static_cast<float>(x * m_levelConfig.CameraMaxViewDistance) };
			const float chunkMaxX{ static_cast<float>(chunkMinX + m_levelConfig.CameraMaxViewDistance) };

			const float chunkMinY{ static_cast<float>(y * m_levelConfig.CameraMaxViewDistance) };
			const float chunkMaxY{ static_cast<float>(chunkMinY + m_levelConfig.CameraMaxViewDistance) };

			// if object is in current chunk
			if (_location.X >= chunkMinX && _location.X < chunkMaxX &&
				_location.Y >= chunkMinY && _location.Y < chunkMaxY)
			{
				// set hit chunk
				m_hitChunk = { static_cast<float>(x), static_cast<float>(y) };
			}
		}
	}

	// check objects in chunk below x and below y of hit chunk for collision with wall
	if (m_hitChunk.X > 0.0f && m_hitChunk.Y > 0.0f)
	{
		for (const std::shared_ptr<Object>& pObject : m_pStaticObjectChunks[static_cast<int>(((m_hitChunk.Y - 1.0f) * m_levelChunkCount.X) + (m_hitChunk.X - 1.0f))])
		{
			if (pObject->GetClassType() == ECLASS_TYPE::WALL)
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
		for (const std::shared_ptr<Object>& pObject : m_pStaticObjectChunks[static_cast<int>(((m_hitChunk.Y - 1.0f) * m_levelChunkCount.X) + m_hitChunk.X)])
		{
			if (pObject->GetClassType() == ECLASS_TYPE::WALL)
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
		for (const std::shared_ptr<Object>& pObject : m_pStaticObjectChunks[static_cast<int>(((m_hitChunk.Y - 1.0f) * m_levelChunkCount.X) + (m_hitChunk.X + 1.0f))])
		{
			if (pObject->GetClassType() == ECLASS_TYPE::WALL)
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
		for (const std::shared_ptr<Object>& pObject : m_pStaticObjectChunks[static_cast<int>((m_hitChunk.Y * m_levelChunkCount.X) + (m_hitChunk.X - 1.0f))])
		{
			if (pObject->GetClassType() == ECLASS_TYPE::WALL)
			{
				// if location is inside the wall return collision true
				if (_location.X >= pObject->GetLocation().X && _location.X <= pObject->GetLocation().X + m_levelConfig.TileSizeCm &&
					_location.Y >= pObject->GetLocation().Y && _location.Y <= pObject->GetLocation().Y + m_levelConfig.TileSizeCm)
					return true;
			}
		}
	}

	// check objects in chunk where the location is of hit chunk for collision with wall
	for (const std::shared_ptr<Object>& pObject : m_pStaticObjectChunks[static_cast<int>((m_hitChunk.Y * m_levelChunkCount.X) + m_hitChunk.X)])
	{
		if (pObject->GetClassType() == ECLASS_TYPE::WALL)
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
		for (const std::shared_ptr<Object>& pObject : m_pStaticObjectChunks[static_cast<int>((m_hitChunk.Y * m_levelChunkCount.X) + (m_hitChunk.X + 1.0f))])
		{
			if (pObject->GetClassType() == ECLASS_TYPE::WALL)
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
		for (const std::shared_ptr<Object>& pObject : m_pStaticObjectChunks[static_cast<int>(((m_hitChunk.Y + 1.0f) * m_levelChunkCount.X) + (m_hitChunk.X - 1.0f))])
		{
			if (pObject->GetClassType() == ECLASS_TYPE::WALL)
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
		for (const std::shared_ptr<Object>& pObject : m_pStaticObjectChunks[static_cast<int>(((m_hitChunk.Y + 1.0f) * m_levelChunkCount.X) + m_hitChunk.X)])
		{
			if (pObject->GetClassType() == ECLASS_TYPE::WALL)
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
		for (const std::shared_ptr<Object>& pObject : m_pStaticObjectChunks[static_cast<int>(((m_hitChunk.Y + 1.0f) * m_levelChunkCount.X) + (m_hitChunk.X + 1.0f))])
		{
			if (pObject->GetClassType() == ECLASS_TYPE::WALL)
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
	m_levelConfig.CameraFOV = { 90.0f, 90.0f };
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
	std::string file{ "Assets/level_" };
	file += std::to_string(Game::Get().GetLevel());
	file += ".cfg";

	std::ifstream infile(file);

	// if file could not open show error scene and return
	if (!infile.is_open())
	{
		std::string errorText{ "Code 10: File " };
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
			std::string errorText{ "Code 12: Separator : is missing in " };
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
			std::string errorText{ "Code 13: Key or Value missing in " };
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
		std::string errorText{ "Code 14: No wall char set in File '" };
		errorText += file;
		errorText += "'";

		Game::Get().ShowErrorScene(errorText);
		m_isLevelConfigLoadError = true;
		return;
	}

	// if no exit char set show error scene and return
	if (m_levelConfig.ExitChar == 32)
	{
		std::string errorText{ "Code 15: No exit char set in File '" };
		errorText += file;
		errorText += "'";

		Game::Get().ShowErrorScene(errorText);
		m_isLevelConfigLoadError = true;
		return;
	}

	// if no player char set show error scene and return
	if (m_levelConfig.PlayerChar == 32)
	{
		std::string errorText{ "Code 16: No player char set in File '" };
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
	std::string file{ "Assets/level_" };
	file += std::to_string(Game::Get().GetLevel());
	file += ".map";

	std::ifstream infile(file);

	// if file could not open show error scene and return
	if (!infile.is_open())
	{
		std::string errorText{ "Code 17: File " };
		errorText += file;
		errorText += " failed to open!";

		Game::Get().ShowErrorScene(errorText);
		return;
	}

	std::string line;

	m_levelChunkCount = { 0, 0 };

	SVector2 exitLocation{ -1.0f, -1.0f };

	const float tileSizeHalf{ m_levelConfig.TileSizeCm * 0.5f };

	while (std::getline(infile, line))
	{
		int levelSizeX{ 0 };

		for (int i{ 0 }; i < line.size(); i++)
		{
			levelSizeX++;

			if(line[i] == ' ')
				continue;

			if (line[i] == m_levelConfig.WallChar)
			{
				m_staticObjects.push_back(std::make_shared<Wall>(SVector2(static_cast<float>(i) * m_levelConfig.TileSizeCm, m_levelChunkCount.Y * m_levelConfig.TileSizeCm)));
			}
			else if (line[i] == m_levelConfig.PlayerChar)
			{
				if (!m_pPlayer)
					m_pPlayer = std::make_shared<Player>(SVector2(i * m_levelConfig.TileSizeCm + tileSizeHalf,
						m_levelChunkCount.Y * m_levelConfig.TileSizeCm + tileSizeHalf));
				else
					m_pPlayer->SetLocation({ i * m_levelConfig.TileSizeCm + tileSizeHalf,
						m_levelChunkCount.Y * m_levelConfig.TileSizeCm + tileSizeHalf });

				m_pPlayer->Init();
			}
			else if (line[i] == m_levelConfig.ExitChar)
			{
				exitLocation = { i * m_levelConfig.TileSizeCm + tileSizeHalf, m_levelChunkCount.Y * m_levelConfig.TileSizeCm + tileSizeHalf };

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
		std::string errorText{ "Code 18: No Player was create from file: '" };
		errorText += file;
		errorText += "'";

		Game::Get().ShowErrorScene(errorText);
		return;
	}

	// if exit location below level show error scene and return
	if (exitLocation.X <= 0.0f || exitLocation.Y <= 0.0f)
	{
		std::string errorText{ "Code 19: Exit Location is below level in map: '" };
		errorText += file;
		errorText += "'";

		Game::Get().ShowErrorScene(errorText);
		return;
	}

	m_pPlayer->SetExitLocation(exitLocation);

	// calculate size of one chunk
	const int tilesPerChunk{ static_cast<int>(m_levelConfig.CameraMaxViewDistance / m_levelConfig.TileSizeCm) };

	// calculate level chunk count
	m_levelChunkCount = { static_cast<float>(static_cast<int>(m_levelChunkCount.X / tilesPerChunk + 1.0f)), static_cast<float>(static_cast<int>(m_levelChunkCount.Y / tilesPerChunk + 1.0f)) };

	m_pStaticObjectChunks = new std::list<std::shared_ptr<Object>>[m_levelChunkCount.X * m_levelChunkCount.Y];
	m_pDynamicObjectChunks = new std::list<std::shared_ptr<Object>>[m_levelChunkCount.X * m_levelChunkCount.Y];
	SplitObjectsInChunks();

	Raytracer::Get().Prepare();
}

void ObjectManager::SplitObjectsInChunks()
{
	const SVector2 playerLocation{ m_pPlayer->GetLocation() };

	for (int y{ 0 }; y < m_levelChunkCount.Y; y++)
	{
		for (int x{ 0 }; x < m_levelChunkCount.X; x++)
		{
			const float chunkMinX{ static_cast<float>(x * m_levelConfig.CameraMaxViewDistance) };
			const float chunkMaxX{ static_cast<float>(chunkMinX + m_levelConfig.CameraMaxViewDistance) };
			const float chunkMinY{ static_cast<float>(y * m_levelConfig.CameraMaxViewDistance) };
			const float chunkMaxY{ static_cast<float>(chunkMinY + m_levelConfig.CameraMaxViewDistance) };

			for (const std::shared_ptr<Object>& pObject : m_staticObjects)
			{
				const float objectX{ pObject->GetLocation().X };
				const float objectY{ pObject->GetLocation().Y };

				if (pObject->GetClassType() == ECLASS_TYPE::WALL)
				{
					// if wall is in current chunk add it to it
					if (((objectX > chunkMinX && objectX < chunkMaxX) ||
						(objectX + m_levelConfig.TileSizeCm > chunkMinX && objectX + m_levelConfig.TileSizeCm < chunkMaxX))
						&&
						((objectY > chunkMinY && objectY < chunkMaxY) ||
							(objectY + m_levelConfig.TileSizeCm > chunkMinY && objectY + m_levelConfig.TileSizeCm < chunkMaxY)))
					{
						m_pStaticObjectChunks[y * static_cast<int>(m_levelChunkCount.X) + x].push_back(pObject);
					}
				}
				else
				{
					// if object is in current chunk add it to it
					if ((objectX >= chunkMinX &&
						objectX < chunkMaxX)
						&&
						(objectY >= chunkMinY &&
							objectY < chunkMaxY))
					{
						m_pStaticObjectChunks[y * static_cast<int>(m_levelChunkCount.X) + x].push_back(pObject);
					}
				}
			}



			// if player is in current chunk set player chunk
			if ((playerLocation.X >= chunkMinX && playerLocation.X < chunkMaxX)
				&&
				(playerLocation.Y >= chunkMinY && playerLocation.Y < chunkMaxY))
			{
				m_playerChunk = { static_cast<float>(x), static_cast<float>(y) };
			}
		}
	}
}

void ObjectManager::ChangePlayerChunk(int _playerChunkX, int _playerChunkY)
{
	// change chunk
	m_playerChunk = { static_cast<float>(_playerChunkX), static_cast<float>(_playerChunkY) };
}