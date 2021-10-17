#pragma once
// C++
#include <string>
#include <memory>

// Window
#include "Window.h"

// Inputs
#include "Keyboard.h"
// #include "Mouse.h"

class Engine
{
public:
	// Constructor
	Engine(LPCWSTR wnd_title, LPCWSTR wnd_class, int width, int height, int x_pos, int y_pos);

	bool ProcessWndMessages();
	LRESULT WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	void Update();

private:
	Window window;
	Keyboard keyboard;

	void RenderFrame();

};

