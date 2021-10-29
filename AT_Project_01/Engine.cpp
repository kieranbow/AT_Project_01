#include "Engine.h"
#include "SceneTest.h"
#include "SceneSwap.h"

Engine::Engine(LPCWSTR wnd_title, LPCWSTR wnd_class, int width, int height, int x_pos, int y_pos) : window(wnd_title, wnd_class, width, height, x_pos, y_pos)
{
	// Render Window
	window.Render(SW_SHOWDEFAULT);

	// Pass window handler to graphics
	pGraphics = std::make_unique<Graphics>(window.GetWindowHandle());

	// Declare Scenes
	std::shared_ptr<SceneTest> sceneTest = std::make_shared<SceneTest>(sceneManager);
	std::shared_ptr<SceneSwap> sceneSwap = std::make_shared<SceneSwap>(sceneManager);

	// Update IDList and create scenes
	sceneManager.IDList.Testing = sceneManager.AddScene(sceneTest, pGraphics.get());
	sceneManager.IDList.swap = sceneManager.AddScene(sceneSwap, pGraphics.get());

	// Switch Scene
	sceneManager.SwitchScene(sceneManager.IDList.Testing);
}

bool Engine::ProcessWndMessages()
{
	return window.ProcessMessages();
}

LRESULT Engine::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		// Keyboard
		case WM_KEYDOWN:
		{
			unsigned char keycode = static_cast<unsigned char>(wParam);

			if (keyboard.IsAutoRepeatKeyOn())
			{
				keyboard.OnKeyPressed(keycode);
			}
			else
			{
				const bool wasPressed = lParam & 0x40000000;
				if (!wasPressed)
				{
					keyboard.OnKeyPressed(keycode);
				}
			}
			return 0;
		}

		case WM_KEYUP:
		{
			unsigned char keycode = static_cast<unsigned char>(wParam);
			keyboard.OnKeyReleased(keycode);
			return 0;
		}

		case WM_CHAR:
		{
			unsigned char ch = static_cast<unsigned char>(wParam);

			if (keyboard.IsAutoRepeatCharOn())
			{
				keyboard.OnChar(ch);
			}
			else
			{
				const bool wasPressed = lParam & 0x40000000;
				if (!wasPressed)
				{
					keyboard.OnChar(ch);
				}
			}
			return 0;
		}

		// ----------Mouse----------

		case WM_MOUSEMOVE:
		{
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);
			mouse.OnMouseMove(x, y);
			return 0;
		}
		// Left Button
		case WM_LBUTTONDOWN:
		{
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);
			mouse.OnLeftBtnPressed(x, y);
			return 0;
		}
		case WM_LBUTTONUP:
		{
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);
			mouse.OnLeftBtnReleased(x, y);
			return 0;
		}

		// Right Button
		case WM_RBUTTONDOWN:
		{
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);
			mouse.OnRightBtnPressed(x, y);
			return 0;
		}
		case WM_RBUTTONUP:
		{
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);
			mouse.OnRightBtnReleased(x, y);
			return 0;
		}

		// Middle Button
		case WM_MBUTTONDOWN:
		{
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);
			mouse.OnMiddleBtnPressed(x, y);
			return 0;
		}
		case WM_MBUTTONUP:
		{
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);
			mouse.OnMiddleBtnReleased(x, y);
			return 0;
		}
		case WM_MOUSEWHEEL:
		{
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);
			if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
			{
				mouse.OnWheelUp(x, y);
			}
			if (GET_WHEEL_DELTA_WPARAM(wParam) < 0)
			{
				mouse.OnWheelDown(x, y);
			}
			return 0;
		}



		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}

void Engine::Input()
{
	sceneManager.Input(keyboard, mouse);
}

void Engine::Update(double dt)
{
	sceneManager.Update(dt);
}

void Engine::RenderFrame()
{
	pGraphics->ClearBuffer(1.0f, 0.5f, 0.0f);

	sceneManager.Draw(pGraphics.get());
	//cube = std::make_unique<DaCube>(pGraphics);
	//cube->Draw(pGraphics->GetDeviceContext());

	//pGraphics->drawTriangle(
		//mouse.GetPosX() / 400.f - 1.0f,
		//-mouse.GetPosY() / 300.f + 1.0f);

	// pGraphics->DrawFrame();

	pGraphics->EndFrame();
}
