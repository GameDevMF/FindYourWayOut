#pragma once

#pragma region include system
#include <list>
#include <memory>
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
	inline SLevelConfig GetLevelConfig() const { return m_levelConfig; }

	/// <summary>
	/// get player
	/// </summary>
	/// <returns>player reference</returns>
	inline Player& GetPlayer() const { return *m_pPlayer.get(); }

	/// <summary>
	/// get all objects
	/// </summary>
	/// <returns>all objects</returns>
	constexpr const std::list<std::shared_ptr<Object>>& GetStaticObjects() const noexcept { return m_staticObjects; }

	/// <summary>
	/// get all dynamic objects
	/// </summary>
	/// <returns>all objects</returns>
	constexpr const std::list<std::shared_ptr<Object>>& GetDynamicObjects() const noexcept { return m_dynamicObjects; }
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
	bool m_isLevelConfigLoadError{ false };
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
	std::shared_ptr<Player> m_pPlayer{ nullptr };

	/// <summary>
	/// chunks of the level with static objects
	/// </summary>
	std::list<std::shared_ptr<Object>>* m_pStaticObjectChunks{ nullptr };

	/// <summary>
	/// chunks of the level with dynamic objects
	/// </summary>
	std::list<std::shared_ptr<Object>>* m_pDynamicObjectChunks{ nullptr };

	/// <summary>
	/// all object in current scene
	/// </summary>
	std::list<std::shared_ptr<Object>> m_staticObjects;

	/// <summary>
	/// all objects that needs to be updated
	/// </summary>
	std::list<std::shared_ptr<Object>> m_dynamicObjects;
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