#include "Engine.h"
#include "ErrorChecker.h"
#include "SceneTest.h"
#include "SceneSwap.h"

Engine::Engine(LPCWSTR wnd_title, LPCWSTR wnd_class, int width, int height, int x_pos, int y_pos) : window(wnd_title, wnd_class, width, height, x_pos, y_pos)
{
	// Render Window
	window.Render(SW_SHOWDEFAULT);

	// Pass window handler to graphics
	pGraphics = std::make_unique<Graphics>(window.GetWindowHandle(), width, height);

	// Create scene Data struct
	sceneData.gfx = pGraphics.get();
	sceneData.keyboard = pKeyboard.get();
	sceneData.mouse = pMouse.get();

	// Declare Scenes
	std::shared_ptr<SceneTest> sceneTest = std::make_shared<SceneTest>(sceneManager);
	std::shared_ptr<SceneSwap> sceneSwap = std::make_shared<SceneSwap>(sceneManager);

	// Update IDList and create scenes
	sceneManager.IDList.Testing = sceneManager.AddScene(sceneTest, sceneData);
	sceneManager.IDList.swap = sceneManager.AddScene(sceneSwap, sceneData);

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

			if (pKeyboard->IsAutoRepeatKeyOn())
			{
				pKeyboard->OnKeyPressed(keycode);
			}
			else
			{
				const bool wasPressed = lParam & 0x40000000;
				if (!wasPressed)
				{
					pKeyboard->OnKeyPressed(keycode);
				}
			}
			return 0;
		}

		case WM_KEYUP:
		{
			unsigned char keycode = static_cast<unsigned char>(wParam);
			pKeyboard->OnKeyReleased(keycode);
			return 0;
		}

		case WM_CHAR:
		{
			unsigned char ch = static_cast<unsigned char>(wParam);

			if (pKeyboard->IsAutoRepeatCharOn())
			{
				pKeyboard->OnChar(ch);
			}
			else
			{
				const bool wasPressed = lParam & 0x40000000;
				if (!wasPressed)
				{
					pKeyboard->OnChar(ch);
				}
			}
			return 0;
		}

		// ----------Mouse----------

		case WM_MOUSEMOVE:
		{
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);
			pMouse->OnMouseMove(x, y);
			return 0;
		}
		// Left Button
		case WM_LBUTTONDOWN:
		{
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);
			pMouse->OnLeftBtnPressed(x, y);
			return 0;
		}
		case WM_LBUTTONUP:
		{
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);
			pMouse->OnLeftBtnReleased(x, y);
			return 0;
		}

		// Right Button
		case WM_RBUTTONDOWN:
		{
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);
			pMouse->OnRightBtnPressed(x, y);
			return 0;
		}
		case WM_RBUTTONUP:
		{
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);
			pMouse->OnRightBtnReleased(x, y);
			return 0;
		}

		// Middle Button
		case WM_MBUTTONDOWN:
		{
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);
			pMouse->OnMiddleBtnPressed(x, y);
			return 0;
		}
		case WM_MBUTTONUP:
		{
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);
			pMouse->OnMiddleBtnReleased(x, y);
			return 0;
		}
		case WM_MOUSEWHEEL:
		{
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);
			if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
			{
				pMouse->OnWheelUp(x, y);
			}
			if (GET_WHEEL_DELTA_WPARAM(wParam) < 0)
			{
				pMouse->OnWheelDown(x, y);
			}
			return 0;
		}

		//case WM_INPUT:
		//{
			//UINT datasize;
			//GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, NULL, &datasize, sizeof(RAWINPUTHEADER));
			
			//if (datasize > 0)
			//{
				//std::unique_ptr<BYTE[]> rawData = std::make_unique<BYTE[]>(datasize);
				//if (GetRawInputData(
					//reinterpret_cast<HRAWINPUT>(lParam), 
					//RID_INPUT, rawData.get(), &datasize, sizeof(RAWINPUTHEADER)) == datasize)
				//{
					//RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(rawData.get());
					//if (raw->header.dwType == RIM_TYPEMOUSE)
					//{
						//mouse.OnMouseMoveRaw(raw->data.mouse.lLastX, raw->data.mouse.lLastY);
					//}
				//}
			//}
			
			//return DefWindowProc(hwnd, msg, wParam, lParam);
		//}

		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}

void Engine::Input()
{
	while (!pKeyboard->IsCharBufferEmpty())
	{
		unsigned char ch = pKeyboard->ReadChar();
	}
	while (!pKeyboard->IsKeyBufferEmpty())
	{
		auto kbe = pKeyboard->ReadKeycode();
		unsigned char keycode = kbe.GetKeyCode();
	}

	while (!pMouse->EventBufferIsEmpty())
	{
		MouseEvent me = pMouse->ReadEvent();
		
		if (me.GetType() == MouseEvent::EventType::WheelDown)
		{
			OutputDebugStringA("wheel down\n");
		}
		if (me.GetType() == MouseEvent::EventType::WheelUp)
		{
			OutputDebugStringA("wheel up\n");
		}

	}

	sceneManager.Input(sceneData);
}

void Engine::Update(double dt)
{
	sceneManager.Update(dt);
}

void Engine::RenderFrame()
{
	pGraphics->ClearBuffer(1.0f, 0.5f, 0.0f);
	sceneManager.Draw(sceneData);
	pGraphics->EndFrame();
}
