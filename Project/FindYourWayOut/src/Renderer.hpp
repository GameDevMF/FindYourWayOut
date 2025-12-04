#pragma once

#pragma region include system
#include <windows.h>
#include <iostream>
#pragma endregion

#pragma region include project
#include "Object.hpp"
#pragma endregion

/// <summary>
/// class to render everything
/// </summary>
class Renderer : public Object
{
public:
#pragma region singleton
	/// <summary>
	/// get renderer instance
	/// </summary>
	/// <returns>renderer instance</returns>
	static Renderer& Get()
	{
		static Renderer instance;
		return instance;
	}
#pragma endregion

#pragma region destructor
	/// <summary>
	/// destructor
	/// </summary>
	~Renderer();
#pragma endregion

#pragma region override method
	/// <summary>
	/// initialize object
	/// </summary>
	virtual void Init() override;

	/// <summary>
	/// update every frame
	/// </summary>
	/// <param name="_deltaSeconds">time since last frame</param>
	virtual void Update(float _deltaSeconds) override {}

	/// <summary>
	/// render every frame
	/// </summary>
	virtual void Render() override;
#pragma endregion

#pragma region inline method
	/// <summary>
	/// get screen with
	/// </summary>
	/// <returns>screen with</returns>
	constexpr int GetScreenWidth() const noexcept { return m_screenWidth; }

	/// <summary>
	/// get screen height
	/// </summary>
	/// <returns>screen height</returns>
	constexpr int GetScreenHeight() const noexcept { return m_screenHeight; }
#pragma endregion

#pragma region method
	/// <summary>
	/// render char at position on screen
	/// </summary>
	/// <param name="_unicodeChar">unicode char</param>
	/// <param name="_attribute">attribute like color</param>
	/// <param name="_pos">position on screen</param>
	void RenderAtPos(const WCHAR& _unicodeChar, const WORD& _attribute, const SVector2& _pos);

	/// <summary>
	/// render sprite at position on screen
	/// </summary>
	/// <param name="_pSprite">sprite to render</param>
	/// <param name="_pos">position on screen</param>
	/// <param name="_width">width of sprite</param>
	/// <param name="_height">height of sprite</param>
	void RenderAtPos(CHAR_INFO const* const _pSprite, const SVector2& _pos, int _width, int _height);
#pragma endregion

private:
#pragma region constructor
	/// <summary>
	/// constructor
	/// </summary>
	Renderer() {};
#pragma endregion

#pragma region primitive variable
	/// <summary>
	/// screen width in chars
	/// </summary>
	int m_screenWidth{ 0 };

	/// <summary>
	/// screen height in chars
	/// </summary>
	int m_screenHeight{ 0 };
#pragma endregion

#pragma region variable
	/// <summary>
	/// title text
	/// </summary>
	wchar_t m_title[256];

	/// <summary>
	/// app name
	/// </summary>
	std::wstring m_appName{ L"FIND YOUR WAY OUT" };

	/// <summary>
	/// original console info to reset to at renderer close
	/// </summary>
	CONSOLE_SCREEN_BUFFER_INFO m_originalConsoleInfo{ CONSOLE_SCREEN_BUFFER_INFO() };

	/// <summary>
	/// window rect to render to
	/// </summary>
	SMALL_RECT m_rectWindow{ SMALL_RECT() };
#pragma endregion

#pragma region pointer
	/// <summary>
	/// reference to original console
	/// </summary>
	HANDLE m_pOriginalConsole{ nullptr };

	/// <summary>
	/// console reference
	/// </summary>
	HANDLE m_pConsole{ nullptr };

	/// <summary>
	/// console input reference
	/// </summary>
	HANDLE m_pConsoleIn{ nullptr };

	/// <summary>
	/// char info array to display on screen
	/// </summary>
	CHAR_INFO* m_pScreen{ nullptr };
#pragma endregion

#pragma region method
	/// <summary>
	/// error to show
	/// </summary>
	/// <param name="_message">message to show</param>
	/// <param name="_errorCode">error code</param>
	void Error(const wchar_t* _message, int _errorCode);
#pragma endregion
};