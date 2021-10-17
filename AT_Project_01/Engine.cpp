#include "Engine.h"

Engine::Engine(LPCWSTR wnd_title, LPCWSTR wnd_class, int width, int height, int x_pos, int y_pos) : window(wnd_title, wnd_class, width, height, x_pos, y_pos)
{
	window.Render(SW_SHOWDEFAULT);
}

bool Engine::ProcessWndMessages()
{
	return window.ProcessMessages() == true;
}

void Engine::Update()
{
	while (!keyboard.IsCharBufferEmpty())
	{
		unsigned char ch = keyboard.ReadChar();
		std::string out_msg = "Char: ";
		out_msg += ch;
		out_msg += "\n";
		OutputDebugStringA(out_msg.c_str());
	}
}

LRESULT Engine::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
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

		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}

void Engine::RenderFrame()
{
	// Clear Buffer

	//SwapChain present
}
