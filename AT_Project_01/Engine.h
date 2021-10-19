#pragma once
// C++
#include <string>
#include <memory>

// Window
#include "Window.h"

// Inputs
#include "Keyboard.h"
#include "Mouse.h"

// Graphics
#include "Graphics.h"

class Engine
{
public:
	// Constructor
	Engine(LPCWSTR wnd_title, LPCWSTR wnd_class, int width, int height, int x_pos, int y_pos);

	bool ProcessWndMessages();
	LRESULT WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	void Update();
	void RenderFrame();

private:
	Window window;

	// Inputs
	Keyboard keyboard;
	Mouse mouse;

	std::unique_ptr<Graphics> pGraphics;
};

