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

// Scene
#include "SceneManager.h"

class Engine
{
public:
	// Constructor
	Engine(LPCWSTR wnd_title, LPCWSTR wnd_class, int width, int height, int x_pos, int y_pos);
	~Engine() = default;

	void Input();
	void Update(double dt);
	void RenderFrame();

	bool ProcessWndMessages();
	LRESULT WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	Window window;
	std::unique_ptr<Keyboard> pKeyboard = std::make_unique<Keyboard>();
	std::unique_ptr<Mouse> pMouse = std::make_unique<Mouse>();
	SceneManager sceneManager;

	SceneData sceneData;

	std::unique_ptr<Graphics> pGraphics;
	std::unique_ptr<Cube> cube;
};
