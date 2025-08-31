#pragma region include project
#include "Enums.hpp"
#include "Game.hpp"
#include "Renderer.hpp"
#pragma endregion

Renderer::~Renderer()
{
	// set console to original console
	SetConsoleActiveScreenBuffer(m_pOriginalConsole);

	delete[] m_pScreen;

	CloseHandle(m_pConsole);
	CloseHandle(m_pConsoleIn);
	CloseHandle(m_pOriginalConsole);
}

void Renderer::Init()
{
	m_screenWidth = 160;
	m_screenHeight = 100;

	m_pConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	m_pConsoleIn = GetStdHandle(STD_INPUT_HANDLE);

	if (m_pConsole == INVALID_HANDLE_VALUE)
	{
		Error(L"Invalid Console Handle", 20);
		return;
	}

	m_rectWindow = { 0, 0, 1, 1 };
	SetConsoleWindowInfo(m_pConsole, true, &m_rectWindow);

	COORD coord = { (short)m_screenWidth, (short)m_screenHeight };
	if (!SetConsoleScreenBufferSize(m_pConsole, coord))
	{
		Error(L"SetConsoleScreenBufferSize", 21);
		return;
	}

	if (!SetConsoleActiveScreenBuffer(m_pConsole))
	{
		Error(L"SetConsoleActiveScreenBuffer", 22);
		return;
	}

	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;

	// font width and height
	cfi.dwFontSize.X = 8;
	cfi.dwFontSize.Y = 8;

	// font family and weight
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;

	// set font to consolas
	wcscpy_s(cfi.FaceName, L"Consolas");

	if (!SetCurrentConsoleFontEx(m_pConsole, false, &cfi))
	{
		Error(L"SetCurrentConsoleFontEx", 23);
		return;
	}

	CONSOLE_SCREEN_BUFFER_INFO csbi;

	if (!GetConsoleScreenBufferInfo(m_pConsole, &csbi))
	{
		Error(L"GetConsoleScreenBufferInfo", 24);
		return;
	}

	// if screen width or height are higher than actual screen return
	if (m_screenWidth > csbi.dwMaximumWindowSize.X)
	{
		Error(L"Screen Width / Font Width Too Big", 25);
		return;
	}
	if (m_screenHeight > csbi.dwMaximumWindowSize.Y)
	{
		Error(L"Screen Height / Font Height Too Big", 26);
		return;
	}

	m_rectWindow = { 0, 0, (short)m_screenWidth - 1, (short)m_screenHeight - 1 };
	if (!SetConsoleWindowInfo(m_pConsole, TRUE, &m_rectWindow))
	{
		Error(L"SetConsoleWindowInfo", 27);
		return;
	}

	// set console input mode
	if (!SetConsoleMode(m_pConsoleIn, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT))
	{
		Error(L"SetConsoleMode", 28);
		return;
	}

	m_pScreen = new CHAR_INFO[m_screenWidth * m_screenHeight];
	memset(m_pScreen, 0, sizeof(CHAR_INFO) * m_screenWidth * m_screenHeight);
}

void Renderer::RenderAtPos(const WCHAR& _unicodeChar, const WORD& _attribute, const SVector2& _pos)
{
	m_pScreen[(static_cast<int>(_pos.Y) * m_screenWidth + static_cast<int>(_pos.X))].Char.UnicodeChar = _unicodeChar;
	m_pScreen[(static_cast<int>(_pos.Y) * m_screenWidth + static_cast<int>(_pos.X))].Attributes = _attribute;
}

void Renderer::RenderAtPos(CHAR_INFO const* const _pSprite, const SVector2& _pos, int _width, int _height)
{
	for (int y = 0; y < _height; y++)
	{
		for (int x = 0; x < _width; x++)
		{
			// if char is 0 continue
			if (!_pSprite[y * _width + x].Char.UnicodeChar)
				continue;

			m_pScreen[(static_cast<int>(_pos.Y + y) * m_screenWidth + static_cast<int>(_pos.X)) + x].Char.UnicodeChar = _pSprite[y * _width + x].Char.UnicodeChar;
			m_pScreen[(static_cast<int>(_pos.Y + y) * m_screenWidth + static_cast<int>(_pos.X)) + x].Attributes = _pSprite[y * _width + x].Attributes;
		}
	}
}

void Renderer::Render()
{
	// display title text
	swprintf_s(m_title, 256, L"%s - FPS: %5.0f", m_appName.c_str(), 1.0f / Game::Get().GetDeltaSeconds());
	SetConsoleTitle(m_title);

	WriteConsoleOutput(m_pConsole, m_pScreen, { (short)m_screenWidth, (short)m_screenHeight }, { 0,0 }, &m_rectWindow);

	// clear screen for next frame
	memset(m_pScreen, 0, sizeof(CHAR_INFO) * m_screenWidth * m_screenHeight);
}

void Renderer::Error(const wchar_t* _message, int _errorCode)
{
	wchar_t buf[256];

	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), buf, 256, NULL);

	SetConsoleActiveScreenBuffer(m_pOriginalConsole);

	wprintf(L"ERROR: %i\n%s\n\t%s\n", _errorCode, _message, buf);
}