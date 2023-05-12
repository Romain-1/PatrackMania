#include "Input.h"
#include "Console.hpp"

void Keyboard::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS) m_keys[key] = JUST_PRESSED;
	if (action == GLFW_RELEASE) m_keys[key] = JUST_RELEASED;
	needUpdate = true;
}

void Keyboard::Update()
{
	if (!needUpdate) return;
	for (auto& key : m_keys)
	{
		if (key.second == JUST_PRESSED) key.second = PRESSED;
		if (key.second == JUST_RELEASED) key.second = RELEASED;
	}
	needUpdate = false;
}

KeyState Keyboard::GetKeyState(int key)
{
	return m_keys[key];
}

bool Keyboard::Any(KeyState state)
{
	for (auto& key : m_keys)
	{
		if (key.second == state) return true;
	}
	return false;
}

void Mouse::ButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS) m_buttons[button] = JUST_PRESSED;
	if (action == GLFW_RELEASE) m_buttons[button] = JUST_RELEASED;
	needUpdate = true;
}

void Mouse::Update()
{
	if (!needUpdate) return;
	for (auto& button : m_buttons)
	{
		if (button.second == JUST_PRESSED) button.second = PRESSED;
		if (button.second == JUST_RELEASED) button.second = RELEASED;
	}
	needUpdate = false;
}

KeyState Mouse::GetButtonState(int button)
{
	return m_buttons[button];
}

bool Mouse::Any(KeyState state)
{
	for (auto& button : m_buttons)
	{
		if (button.second == state) return true;
	}
	return false;
}

void Mouse::CursorPositionCallBack(GLFWwindow* window, double xpos, double ypos)
{
	Mouse::cursor = glm::vec2(xpos, ypos);
}