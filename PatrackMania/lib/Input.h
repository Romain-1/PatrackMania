#pragma once
#include <map>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>

enum KeyState
{
	RELEASED = 0,
	JUST_PRESSED = 1 << 0,
	PRESSED = 1 << 1,
	JUST_RELEASED = 1 << 2
};

class Mouse
{
public:
	static void CursorPositionCallBack(GLFWwindow* window, double xpos, double ypos);
	static void ButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static KeyState GetButtonState(int button);
	static bool Any(KeyState state);
	static void Update();

	static inline glm::vec2 cursor = glm::vec2(0, 0);

private:
	static inline bool needUpdate = false;
	static inline std::map<int, KeyState> m_buttons{
		{ GLFW_MOUSE_BUTTON_1, KeyState::RELEASED },
		{ GLFW_MOUSE_BUTTON_2, KeyState::RELEASED },
		{ GLFW_MOUSE_BUTTON_3, KeyState::RELEASED },
		{ GLFW_MOUSE_BUTTON_4, KeyState::RELEASED },
		{ GLFW_MOUSE_BUTTON_5, KeyState::RELEASED },
		{ GLFW_MOUSE_BUTTON_6, KeyState::RELEASED },
		{ GLFW_MOUSE_BUTTON_7, KeyState::RELEASED },
		{ GLFW_MOUSE_BUTTON_8, KeyState::RELEASED }
	};
};

class Keyboard
{
public:
	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static KeyState GetKeyState(int key);
	static bool Any(KeyState state);
	static void Update();

private:
	static inline bool needUpdate = false;
	static inline std::map<int, KeyState> m_keys{
		{ GLFW_KEY_UNKNOWN, KeyState::RELEASED},
		{ GLFW_KEY_SPACE, KeyState::RELEASED},
		{ GLFW_KEY_APOSTROPHE, KeyState::RELEASED},
		{ GLFW_KEY_COMMA, KeyState::RELEASED},
		{ GLFW_KEY_MINUS, KeyState::RELEASED},
		{ GLFW_KEY_PERIOD, KeyState::RELEASED},
		{ GLFW_KEY_SLASH, KeyState::RELEASED},
		{ GLFW_KEY_0, KeyState::RELEASED},
		{ GLFW_KEY_1, KeyState::RELEASED},
		{ GLFW_KEY_2, KeyState::RELEASED},
		{ GLFW_KEY_3, KeyState::RELEASED},
		{ GLFW_KEY_4, KeyState::RELEASED},
		{ GLFW_KEY_5, KeyState::RELEASED},
		{ GLFW_KEY_6, KeyState::RELEASED},
		{ GLFW_KEY_7, KeyState::RELEASED},
		{ GLFW_KEY_8, KeyState::RELEASED},
		{ GLFW_KEY_9, KeyState::RELEASED},
		{ GLFW_KEY_SEMICOLON, KeyState::RELEASED},
		{ GLFW_KEY_EQUAL, KeyState::RELEASED},
		{ GLFW_KEY_A, KeyState::RELEASED},
		{ GLFW_KEY_B, KeyState::RELEASED},
		{ GLFW_KEY_C, KeyState::RELEASED},
		{ GLFW_KEY_D, KeyState::RELEASED},
		{ GLFW_KEY_E, KeyState::RELEASED},
		{ GLFW_KEY_F, KeyState::RELEASED},
		{ GLFW_KEY_G, KeyState::RELEASED},
		{ GLFW_KEY_H, KeyState::RELEASED},
		{ GLFW_KEY_I, KeyState::RELEASED},
		{ GLFW_KEY_J, KeyState::RELEASED},
		{ GLFW_KEY_K, KeyState::RELEASED},
		{ GLFW_KEY_L, KeyState::RELEASED},
		{ GLFW_KEY_M, KeyState::RELEASED},
		{ GLFW_KEY_N, KeyState::RELEASED},
		{ GLFW_KEY_O, KeyState::RELEASED},
		{ GLFW_KEY_P, KeyState::RELEASED},
		{ GLFW_KEY_Q, KeyState::RELEASED},
		{ GLFW_KEY_R, KeyState::RELEASED},
		{ GLFW_KEY_S, KeyState::RELEASED},
		{ GLFW_KEY_T, KeyState::RELEASED},
		{ GLFW_KEY_U, KeyState::RELEASED},
		{ GLFW_KEY_V, KeyState::RELEASED},
		{ GLFW_KEY_W, KeyState::RELEASED},
		{ GLFW_KEY_X, KeyState::RELEASED},
		{ GLFW_KEY_Y, KeyState::RELEASED},
		{ GLFW_KEY_Z, KeyState::RELEASED},
		{ GLFW_KEY_LEFT_BRACKET, KeyState::RELEASED},
		{ GLFW_KEY_BACKSLASH, KeyState::RELEASED},
		{ GLFW_KEY_RIGHT_BRACKET, KeyState::RELEASED},
		{ GLFW_KEY_GRAVE_ACCENT, KeyState::RELEASED},
		{ GLFW_KEY_WORLD_1, KeyState::RELEASED},
		{ GLFW_KEY_WORLD_2, KeyState::RELEASED},
		{ GLFW_KEY_ESCAPE, KeyState::RELEASED},
		{ GLFW_KEY_ENTER, KeyState::RELEASED},
		{ GLFW_KEY_TAB, KeyState::RELEASED},
		{ GLFW_KEY_BACKSPACE, KeyState::RELEASED},
		{ GLFW_KEY_INSERT, KeyState::RELEASED},
		{ GLFW_KEY_DELETE, KeyState::RELEASED},
		{ GLFW_KEY_RIGHT, KeyState::RELEASED},
		{ GLFW_KEY_LEFT, KeyState::RELEASED},
		{ GLFW_KEY_DOWN, KeyState::RELEASED},
		{ GLFW_KEY_UP, KeyState::RELEASED},
		{ GLFW_KEY_PAGE_UP, KeyState::RELEASED},
		{ GLFW_KEY_PAGE_DOWN, KeyState::RELEASED},
		{ GLFW_KEY_HOME, KeyState::RELEASED},
		{ GLFW_KEY_END, KeyState::RELEASED},
		{ GLFW_KEY_CAPS_LOCK, KeyState::RELEASED},
		{ GLFW_KEY_SCROLL_LOCK, KeyState::RELEASED},
		{ GLFW_KEY_NUM_LOCK, KeyState::RELEASED},
		{ GLFW_KEY_PRINT_SCREEN, KeyState::RELEASED},
		{ GLFW_KEY_PAUSE, KeyState::RELEASED},
		{ GLFW_KEY_F1, KeyState::RELEASED},
		{ GLFW_KEY_F2, KeyState::RELEASED},
		{ GLFW_KEY_F3, KeyState::RELEASED},
		{ GLFW_KEY_F4, KeyState::RELEASED},
		{ GLFW_KEY_F5, KeyState::RELEASED},
		{ GLFW_KEY_F6, KeyState::RELEASED},
		{ GLFW_KEY_F7, KeyState::RELEASED},
		{ GLFW_KEY_F8, KeyState::RELEASED},
		{ GLFW_KEY_F9, KeyState::RELEASED},
		{ GLFW_KEY_F10, KeyState::RELEASED},
		{ GLFW_KEY_F11, KeyState::RELEASED},
		{ GLFW_KEY_F12, KeyState::RELEASED},
		{ GLFW_KEY_F13, KeyState::RELEASED},
		{ GLFW_KEY_F14, KeyState::RELEASED},
		{ GLFW_KEY_F15, KeyState::RELEASED},
		{ GLFW_KEY_F16, KeyState::RELEASED},
		{ GLFW_KEY_F17, KeyState::RELEASED},
		{ GLFW_KEY_F18, KeyState::RELEASED},
		{ GLFW_KEY_F19, KeyState::RELEASED},
		{ GLFW_KEY_F20, KeyState::RELEASED},
		{ GLFW_KEY_F21, KeyState::RELEASED},
		{ GLFW_KEY_F22, KeyState::RELEASED},
		{ GLFW_KEY_F23, KeyState::RELEASED},
		{ GLFW_KEY_F24, KeyState::RELEASED},
		{ GLFW_KEY_F25, KeyState::RELEASED},
		{ GLFW_KEY_KP_0, KeyState::RELEASED},
		{ GLFW_KEY_KP_1, KeyState::RELEASED},
		{ GLFW_KEY_KP_2, KeyState::RELEASED},
		{ GLFW_KEY_KP_3, KeyState::RELEASED},
		{ GLFW_KEY_KP_4, KeyState::RELEASED},
		{ GLFW_KEY_KP_5, KeyState::RELEASED},
		{ GLFW_KEY_KP_6, KeyState::RELEASED},
		{ GLFW_KEY_KP_7, KeyState::RELEASED},
		{ GLFW_KEY_KP_8, KeyState::RELEASED},
		{ GLFW_KEY_KP_9, KeyState::RELEASED},
		{ GLFW_KEY_KP_DECIMAL, KeyState::RELEASED},
		{ GLFW_KEY_KP_DIVIDE, KeyState::RELEASED},
		{ GLFW_KEY_KP_MULTIPLY, KeyState::RELEASED},
		{ GLFW_KEY_KP_SUBTRACT, KeyState::RELEASED},
		{ GLFW_KEY_KP_ADD, KeyState::RELEASED},
		{ GLFW_KEY_KP_ENTER, KeyState::RELEASED},
		{ GLFW_KEY_KP_EQUAL, KeyState::RELEASED},
		{ GLFW_KEY_LEFT_SHIFT, KeyState::RELEASED},
		{ GLFW_KEY_LEFT_CONTROL, KeyState::RELEASED},
		{ GLFW_KEY_LEFT_ALT, KeyState::RELEASED},
		{ GLFW_KEY_LEFT_SUPER, KeyState::RELEASED},
		{ GLFW_KEY_RIGHT_SHIFT, KeyState::RELEASED},
		{ GLFW_KEY_RIGHT_CONTROL, KeyState::RELEASED},
		{ GLFW_KEY_RIGHT_ALT, KeyState::RELEASED},
		{ GLFW_KEY_RIGHT_SUPER, KeyState::RELEASED},
		{ GLFW_KEY_MENU, KeyState::RELEASED},
		{ GLFW_KEY_LAST, KeyState::RELEASED}
	};
};
