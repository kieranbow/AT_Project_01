#pragma once
#include "Window.h"

// #include "Keyboard.h"
// #include "Mouse.h"

class Engine
{
public:
	// Constructor
	Engine(LPCWSTR wnd_title, LPCWSTR wnd_class, int width, int height, int x_pos, int y_pos);

	bool ProcessWndMessages();

	void Update();

private:
	Window window;

	void RenderFrame();

};

