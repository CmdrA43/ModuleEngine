#include "input.h"

namespace frame {

	Input::KeyboardInputMap Input::keyboard;

	Input::KeyState Input::getKeyState(uint32_t keycode) {
		return keyboard.keys[keycode];
	}

	bool Input::isKeyPressed(uint32_t keycode) {
		return keyboard.keys[keycode].isDown;
	}

	bool Input::isKeyReleased(uint32_t keycode) {
		return !keyboard.keys[keycode].isDown;
	
	}

	//returns true if key was just hit
	bool Input::wasKeyHit(uint32_t keycode) {
		return ((!keyboard.keys[keycode].wasDown) && (keyboard.keys[keycode].isDown));
	}

	void Input::processKeyboardInput(uint32_t VKCode, bool wasDown, bool isDown) {
		if (wasDown != isDown) {
			if (VKCode >= 'A' && VKCode <= 'Z') {
				uint32_t frameKeycode = VKCode - 'A';
				keyboard.keys[frameKeycode].isDown = isDown;
				keyboard.keys[frameKeycode].wasDown = wasDown;
			}
			else if (VKCode == VK_UP)
			{
				keyboard.keys[F_UP].isDown = isDown;
				keyboard.keys[F_UP].wasDown = wasDown;
			}
			else if (VKCode == VK_DOWN)
			{
				keyboard.keys[F_DOWN].isDown = isDown;
				keyboard.keys[F_DOWN].wasDown = wasDown;
			}
			else if (VKCode == VK_LEFT)
			{
				keyboard.keys[F_LEFT].isDown = isDown;
				keyboard.keys[F_LEFT].wasDown = wasDown;
			}
			else if (VKCode == VK_RIGHT)
			{
				keyboard.keys[F_RIGHT].isDown = isDown;
				keyboard.keys[F_RIGHT].wasDown = wasDown;
			}
			else if (VKCode >= '0' && VKCode <= '9')
			{
				uint32_t F_keycode = VKCode - '0' + F_0;
				keyboard.keys[F_keycode].isDown = isDown;
				keyboard.keys[F_keycode].wasDown = wasDown;
			}
			else if (VKCode == VK_OEM_MINUS)
			{
				keyboard.keys[F_MINUS].isDown = isDown;
				keyboard.keys[F_MINUS].wasDown = wasDown;
			}
			else if (VKCode == VK_OEM_PLUS)
			{
				keyboard.keys[F_PLUS].isDown = isDown;
				keyboard.keys[F_PLUS].wasDown = wasDown;
			}
			else if (VKCode == VK_SHIFT)
			{
				keyboard.keys[F_SHIFT].isDown = isDown;
				keyboard.keys[F_SHIFT].wasDown = wasDown;
			}
			else if (VKCode == VK_CONTROL)
			{
				keyboard.keys[F_CONTROL].isDown = isDown;
				keyboard.keys[F_CONTROL].wasDown = wasDown;
			}
			else if (VKCode == VK_MENU)
			{
				keyboard.keys[F_ALT].isDown = isDown;
				keyboard.keys[F_ALT].wasDown = wasDown;
			}
			else if (VKCode == VK_SPACE)
			{
				keyboard.keys[F_SPACE].isDown = isDown;
				keyboard.keys[F_SPACE].wasDown = wasDown;
			}
			else if (VKCode == VK_ESCAPE)
			{
				keyboard.keys[F_ESCAPE].isDown = isDown;
				keyboard.keys[F_ESCAPE].wasDown = wasDown;
			}
			else if (VKCode == VK_CAPITAL)
			{
				keyboard.keys[F_CAPSLOCK].isDown = isDown;
				keyboard.keys[F_CAPSLOCK].wasDown = wasDown;
			}
			else if (VKCode == VK_TAB)
			{
				keyboard.keys[F_TAB].isDown = isDown;
				keyboard.keys[F_TAB].wasDown = wasDown;
			}
			else if (VKCode == VK_RETURN)
			{
				keyboard.keys[F_ENTER].isDown = isDown;
				keyboard.keys[F_ENTER].wasDown = wasDown;
			}
			else if (VKCode == VK_BACK)
			{
				keyboard.keys[F_BACKSPACE].isDown = isDown;
				keyboard.keys[F_BACKSPACE].wasDown = wasDown;
			}
			else if (VKCode == VK_OEM_3)
			{
				keyboard.keys[F_TILDE].isDown = isDown;
				keyboard.keys[F_TILDE].wasDown = wasDown;
			}
		}
	}
}