#pragma once

#pragma region include system
#include <windows.h>
#pragma endregion

/// <summary>
/// class to handle input
/// </summary>
class Input
{
public:
	#pragma region inline method
	/// <summary>
	/// if key is pressed
	/// </summary>
	/// <param name="_key">key to check</param>
	/// <returns>if key is pressed</returns>
	inline static const bool& IsKeyPressed(const short& _key)
	{
		return m_keyStateThisFrame[_key] & 0x8000;
	}

	/// <summary>
	/// if key is pressed this frame
	/// </summary>
	/// <param name="_key">key to check</param>
	/// <returns>key is pressed this frame</returns>
	inline static const bool& IsKeyPressedThisFrame(const short& _key)
	{
		return m_keyStateThisFrame[_key] & 0x8000 && !(m_keyStateLastFrame[_key] & 0x8000);
	}

	/// <summary>
	/// if key is released
	/// </summary>
	/// <param name="_key">key to check</param>
	/// <returns>if key is released</returns>
	inline static const bool& IsKeyReleased(const short& _key)
	{
		return !(m_keyStateThisFrame[_key] & 0x8000);
	}

	/// <summary>
	/// if key is released this frame
	/// </summary>
	/// <param name="_key">key to check</param>
	/// <returns>if key is released this frame</returns>
	inline static const bool& IsKeyReleasedThisFrame(const short& _key)
	{
		return !(m_keyStateThisFrame[_key] & 0x8000) && m_keyStateLastFrame[_key] & 0x8000;
	}
	#pragma endregion

	#pragma region method
	/// <summary>
	/// update input at begin frame
	/// </summary>
	static void UpdateBeginFrame()
	{
		for (int i = 0; i < 256; i++)
			m_keyStateLastFrame[i] = m_keyStateThisFrame[i];

		for (int i = 0; i < 256; i++)
			m_keyStateThisFrame[i] = GetAsyncKeyState(i);
	}
	#pragma endregion

private:
	#pragma region primitive variable
	/// <summary>
	/// every key state of last frame
	/// </summary>
	static short m_keyStateLastFrame[256];

	/// <summary>
	/// every key state of this frame
	/// </summary>
	static short m_keyStateThisFrame[256];
	#pragma endregion
};