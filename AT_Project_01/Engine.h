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

#include "DaCube.h"

class Engine
{
public:
	// Constructor
	Engine(LPCWSTR wnd_title, LPCWSTR wnd_class, int width, int height, int x_pos, int y_pos);

	void InitViewport();
	void InitScene();
	void Input();
	void Update();
	void RenderFrame();

	bool ProcessWndMessages();
	LRESULT WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	Window window;
	Keyboard keyboard;
	Mouse mouse;
	std::unique_ptr<Graphics> pGraphics;

	std::unique_ptr<DaCube> cube;

};

