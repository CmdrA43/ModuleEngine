#include "game.h"
#include "renderer.h"
#include "input.h"

namespace frame {
	Game::Game() {
		// init window properties to default values
		windowTitle = L"Frame Application";
		windowWidth = 1280;
		windowHeight = 720;
	}

	// handles window events
	LRESULT CALLBACK WindowCallBack(
		HWND windowHandle,
		UINT message,
		WPARAM wParam,
		LPARAM lParam
	) {
		LRESULT result = 0;

		switch (message) {
			case WM_CLOSE: {
				Game::getInstance().running = false;
				OutputDebugString(L"window close\n");
			}break;

			case WM_DESTROY: {
				Game::getInstance().running = false;
				OutputDebugString(L"window destroy\n");
			}break;

			case WM_SYSKEYDOWN:
			case WM_SYSKEYUP:
			case WM_KEYDOWN:
			case WM_KEYUP:
			{
				uint32_t VKcode = wParam;

				bool wasDown = (lParam & (1 << 30)) != 0;
				bool isDown = (lParam & (1 << 31)) == 0;

				frame::Input::processKeyboardInput(VKcode, wasDown, isDown);
			}break;


			case WM_PAINT: {
				OutputDebugString(L"window paint\n");
				
				PAINTSTRUCT paint;
				HDC device_context = BeginPaint(windowHandle, &paint);

				int width, height;
				Renderer::getWindowDimensions(&width, &height);

				Renderer::copyBufferToWindow(device_context, width, height);

				EndPaint(windowHandle, &paint);
			}break;

			default:
				result = DefWindowProc(windowHandle, message, wParam, lParam);
		}

		return result;
	}

	void Game::startWindow() {
		Renderer::resizeFrameBuffer(windowWidth, windowHeight);

		const wchar_t* className = L"frame_window";

		WNDCLASS windowClass = {};

		windowClass.style = CS_HREDRAW | CS_VREDRAW;
		windowClass.lpfnWndProc = WindowCallBack;
		windowClass.hInstance = hInstance;
		windowClass.lpszClassName = className;

		if (!RegisterClass(&windowClass)) {
			OutputDebugString(L"Failed to register window class\n");
			return;
		}

		windowHandle = CreateWindowEx(
			0,
			className,
			windowTitle.c_str(),
			WS_OVERLAPPEDWINDOW | WS_VISIBLE,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			windowWidth,
			windowHeight,
			0,
			0,
			hInstance,
			0
		);

		if (windowHandle) {
			OutputDebugString(L"GAME INIT\n");
			running = true;

			Renderer::setWindowHandle(windowHandle);

			// init the clock
			auto lastFrameTime = std::chrono::high_resolution_clock::now();


			while (running) {
				auto currentTime = std::chrono::high_resolution_clock::now();
				deltaTime = currentTime - lastFrameTime;


				MSG message;
				while (PeekMessage(&message, 0, 0, 0, PM_REMOVE)) {
					if (message.message == WM_QUIT)
						running = false;

					TranslateMessage(&message);
					DispatchMessage(&message); // sends message to WindowProc (WindowCallBack)
				}

				// update & render

				Renderer::clear();

				getInstance().update(deltaTime.count());

				HDC deviceContext = GetDC(windowHandle);

				int width, height;
				Renderer::getWindowDimensions(&width, &height);

				Renderer::copyBufferToWindow(deviceContext, width, height);

				lastFrameTime = currentTime;

				ReleaseDC(windowHandle, deviceContext);
			}

		}
		else {
			OutputDebugString(L"Failed to create a window\n");
		}
	}
}