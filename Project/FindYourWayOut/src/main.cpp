#pragma once

#ifndef UNICODE
#error Please enable UNICODE for your compiler! \
VS: Project Properties -> General -> Character Set -> Use Unicode.
#endif

#pragma region include project
#include "Game.hpp"
#pragma endregion

/// <summary>
/// main entry point
/// </summary>
/// <param name="argc">number of arguments</param>
/// <param name="argv">arguments array</param>
/// <returns>shutdown code</returns>
int main(int argc, char* argv[])
{
	Game::Get().Init();

	while (Game::Get().IsRunning())
	{
		Game::Get().Update();
		Game::Get().Render();
	}

	return 0;
}