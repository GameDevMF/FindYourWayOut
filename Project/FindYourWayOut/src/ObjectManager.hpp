#pragma once

#pragma region include system
#include <list>
#pragma endregion

#pragma region include project
#include "Structs.hpp"
#include "Player.hpp"
#pragma endregion

/// <summary>
/// class to handle all objects in a level
/// </summary>
class ObjectManager : public Object
{
public:
	#pragma region singleton
	/// <summary>
	/// get game instance
	/// </summary>
	/// <returns>game instance</returns>
	static ObjectManager& Get()
	{
		static ObjectManager instance;
		return instance;
	}
	#pragma endregion

	#pragma region override method
	/// <summary>
	/// initialize object
	/// </summary>
	virtual void Init() override {}

	/// <summary>
	/// update every frame
	/// </summary>
	/// <param name="_deltaSeconds">time since last frame</param>
	virtual void Update(float _deltaSeconds) override;

	/// <summary>
	/// render every frame
	/// </summary>
	virtual void Render() override;
	#pragma endregion

	#pragma region inline method
	/// <summary>
	/// get level config
	/// </summary>
	/// <returns>level config</returns>
	inline const SLevelConfig const GetLevelConfig() { return m_levelConfig; }

	/// <summary>
	/// get player
	/// </summary>
	/// <returns>player reference</returns>
	inline Player& const GetPlayer() { return *m_pPlayer; }

	/// <summary>
	/// get all objects
	/// </summary>
	/// <returns>all objects</returns>
	inline const std::list<Object*> const GetStaticObjects() { return m_pStaticObjects; }

	/// <summary>
	/// get all dynamic objects
	/// </summary>
	/// <returns>all objects</returns>
	inline const std::list<Object*> const GetDynamicObjects() { return m_pDynamicObjects; }
	#pragma endregion

	#pragma region method
	/// <summary>
	/// load level and create objects
	/// </summary>
	void LoadLevel();
	
	/// <summary>
	/// clear all objects from level
	/// </summary>
	/// <param name="_keepPlayer">if player should be deleted or not</param>
	void ClearLevel(bool _keepPlayer);

	/// <summary>
	/// remove single object
	/// </summary>
	/// <param name="_pObject"></param>
	void RemoveObject(Object* _pObject);

	/// <summary>
	/// if given location hits a wall
	/// </summary>
	/// <param name="_location">location to check</param>
	/// <returns>if location is in a wall</returns>
	bool HitWall(SVector2 _location);
	#pragma endregion

private:
	#pragma region constructor
	/// <summary>
	/// constructor
	/// </summary>
	ObjectManager() {}
	#pragma endregion

	#pragma region primitive variable
	/// <summary>
	/// if an error occurred at level config load from file
	/// </summary>
	bool m_isLevelConfigLoadError = false;
	#pragma endregion

	#pragma region variable
	/// <summary>
	/// level chunk count
	/// </summary>
	SVector2 m_levelChunkCount;

	/// <summary>
	/// level chunk the player is in
	/// </summary>
	SVector2 m_playerChunk;

	/// <summary>
	/// level chunk for hit detection
	/// </summary>
	SVector2 m_hitChunk;

	/// <summary>
	/// level config
	/// </summary>
	SLevelConfig m_levelConfig;
	#pragma endregion

	#pragma region pointer
	/// <summary>
	/// player reference
	/// </summary>
	Player* m_pPlayer = nullptr;

	/// <summary>
	/// list of all objects to remove at end of frame
	/// </summary>
	std::list<Object*> m_objectsToRemove;

	/// <summary>
	/// chunks of the level with static objects
	/// </summary>
	std::list<Object*>* m_pStaticObjectChunks = nullptr;

	/// <summary>
	/// chunks of the level with dynamic objects
	/// </summary>
	std::list<Object*>* m_pDynamicObjectChunks = nullptr;

	/// <summary>
	/// all object in current scene
	/// </summary>
	std::list<Object*> m_pStaticObjects;

	/// <summary>
	/// all objects that needs to be updated
	/// </summary>
	std::list<Object*> m_pDynamicObjects;
	#pragma endregion

	#pragma region method
	/// <summary>
	/// reset level config
	/// </summary>
	void ResetLevelConfig();

	/// <summary>
	/// load level config from file
	/// </summary>
	void LoadLevelConfigFromFile();

	/// <summary>
	/// load level map from file
	/// </summary>
	void LoadLevelMapFromFile();

	/// <summary>
	/// split all objects in chunks
	/// </summary>
	void SplitObjectsInChunks();

	/// <summary>
	/// change player chunk
	/// </summary>
	/// <param name="_playerChunkX">new player chunk x</param>
	/// <param name="_playerChunkY">new player chunk y</param>
	void ChangePlayerChunk(int _playerChunkX, int _playerChunkY);
	#pragma endregion
};