#pragma once

#pragma region include system
#include <windows.h>
#pragma endregion

#pragma region include project
#include "Scene.hpp"
#pragma endregion

/// <summary>
/// scene to show error class
/// </summary>
class SceneError : public Scene
{
public:
	#pragma region destructor
	/// <summary>
	/// destructor
	/// </summary>
	~SceneError();
	#pragma endregion

	#pragma region override method
	/// <summary>
	/// initialize scene
	/// </summary>
	virtual void Init() override;

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

	#pragma region mehtod
	/// <summary>
	/// set error details text
	/// </summary>
	/// <param name="_pDetails">error details text</param>
	void SetErrorDetails(const char* _pDetails);
	#pragma endregion

private:
	#pragma region primitive variable
	/// <summary>
	/// length of details text
	/// </summary>
	int m_detailsTextLength = 0;
	#pragma endregion

	#pragma region variable
	/// <summary>
	/// key text position on screen
	/// </summary>
	SVector2 m_keyTextPosition;

	/// <summary>
	/// details text position on screen
	/// </summary>
	SVector2 m_detailsTextPosition;
	#pragma endregion

	#pragma region pointer
	/// <summary>
	/// error text reference
	/// </summary>
	CHAR_INFO* m_pError = nullptr;

	/// <summary>
	/// key text reference
	/// </summary>
	CHAR_INFO* m_pKey = nullptr;

	/// <summary>
	/// details text reference
	/// </summary>
	CHAR_INFO* m_pDetails = nullptr;
	#pragma endregion
};