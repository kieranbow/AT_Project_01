#include "Engine.h"

Engine::Engine(LPCWSTR wnd_title, LPCWSTR wnd_class, int width, int height, int x_pos, int y_pos) : window(wnd_title, wnd_class, width, height, x_pos, y_pos)
{
	window.Render(SW_SHOWDEFAULT);
	pGraphics = std::make_unique<Graphics>(window.GetWindowHandle());
}

bool Engine::ProcessWndMessages()
{
	return window.ProcessMessages();
}

void Engine::Update()
{
	// Remove this
	while (!keyboard.IsCharBufferEmpty())
	{
		unsigned char ch = keyboard.ReadChar();
		std::string out_msg = "Char: ";
		out_msg += ch;
		out_msg += "\n";
		OutputDebugStringA(out_msg.c_str());
	}

	// Remove this
	while (!keyboard.IsKeyBufferEmpty())
	{
		unsigned char keycode = keyboard.ReadKeycode().GetKeyCode();
		std::string out_msg = "Keycode: ";
		out_msg += keycode;
		out_msg += "\n";
		OutputDebugStringA(out_msg.c_str());
	}

	// Remove this
	while (!mouse.EventBufferIsEmpty())
	{
		Mouse::MouseEvent msEvent = mouse.ReadEvent();
		if (msEvent.GetType() == Mouse::MouseEvent::EventType::WheelUp)
		{
			OutputDebugStringA("MouseWheel Up\n");
		}
		if (msEvent.GetType() == Mouse::MouseEvent::EventType::WheelDown)
		{
			OutputDebugStringA("MouseWheel Down\n");
		}
	}
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

void Engine::RenderFrame()
{
	pGraphics->ClearBuffer(1.0f, 0.5f, 0.0f);
	pGraphics->SetPosition(
		mouse.GetPosX() / 400.f - 1.0f, 
		-mouse.GetPosY() / 300.f + 1.0f);

	pGraphics->drawTriangle();
	pGraphics->EndFrame();
}
