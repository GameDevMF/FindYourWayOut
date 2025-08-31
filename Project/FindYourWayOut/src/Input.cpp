#pragma region include project
#include "Input.hpp"
#pragma endregion

short Input::m_keyStateThisFrame[256] = { 0 };
short Input::m_keyStateLastFrame[256] = { 0 };