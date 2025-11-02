#pragma once

#include <windows.h>
#include <stdint.h>

#define F_MAX_KEYS 52

#define F_A			0
#define F_B			1
#define F_C			2
#define F_D			3
#define F_E			4
#define F_F			5
#define F_G			6
#define F_H			7
#define F_I			8
#define F_J			9
#define F_K			10
#define F_L			11
#define F_M			12
#define F_N			13
#define F_O			14
#define F_P			15
#define F_Q			16
#define F_R			17
#define F_S			18
#define F_T			19
#define F_U			20
#define F_V			21
#define F_W			22
#define F_X			23
#define F_Y			24
#define F_Z			25

#define F_UP		26
#define F_DOWN		27
#define F_LEFT		28
#define F_RIGHT		29

#define F_0			30
#define F_1			31
#define F_2			32
#define F_3			33
#define F_4			34
#define F_5			35
#define F_6			36
#define F_7			37
#define F_8			38
#define F_9			39
#define F_MINUS		40
#define F_PLUS		41

#define F_SHIFT		42
#define F_CONTROL	43
#define F_ALT		44
#define F_SPACE		45
#define F_ESCAPE	46
#define F_CAPSLOCK	47
#define F_TAB		48
#define F_ENTER		49
#define F_BACKSPACE	50
#define F_TILDE		51

namespace frame {
	class Input {
		friend LRESULT CALLBACK WindowCallBack(
			HWND windowHandle,
			UINT message,
			WPARAM wParam,
			LPARAM lParam
		);

		private:
			static void processKeyboardInput(uint32_t VKCode, bool wasDown, bool isDown);

		public:
			struct KeyState {
				bool wasDown, isDown;
			};

			struct KeyboardInputMap {
				KeyState keys[F_MAX_KEYS];
			};

		public:
			static KeyState getKeyState(uint32_t keycode);

			static bool isKeyPressed(uint32_t keycode);

			static bool isKeyReleased(uint32_t keycode);

			//returns true if key was just hit
			static bool wasKeyHit(uint32_t keycode);

		private:
			static KeyboardInputMap keyboard;
	};
}