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

// Scene
#include "SceneManager.h"

// Main class for the engine. All game logic including rendering, updating and inputs will be used heres
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
	SceneData sceneData;
	SceneManager sceneManager;

	std::unique_ptr<Keyboard> pKeyboard = std::make_unique<Keyboard>();
	std::unique_ptr<Mouse> pMouse = std::make_unique<Mouse>();
	std::unique_ptr<Graphics> pGraphics;
};
