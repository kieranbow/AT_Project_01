#include "Engine.h"

Engine::Engine(LPCWSTR wnd_title, LPCWSTR wnd_class, int width, int height, int x_pos, int y_pos) : window(wnd_title, wnd_class, width, height, x_pos, y_pos)
{
	window.Render(SW_SHOWDEFAULT);
}

bool Engine::ProcessWndMessages()
{
	return window.ProcessMessages() == true;
}

void Engine::RenderFrame()
{
	// Clear Buffer

	//SwapChain present
}
