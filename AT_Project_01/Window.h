#pragma once
// Windows
#include "WinHeader.h"

// C++
#include <string>

class Engine;

struct V2Int
{
	V2Int(int _x, int _y) : x(_x), y(_y) {}
	int x;
	int y;
};

struct V2Uint
{
	V2Uint(unsigned int _x, unsigned int _y) : x(_x), y(_y) {}
	unsigned int x;
	unsigned int y;
};

// Description
// A window class that handles all window behaviours like rendering, processing messages.
class Window
{
	public:
		// Description
		// Constructor for a window that sets window propertices like width, height, position on screen, class name and title.
		// It will also create the window using the CreateWindowEx() function.
		Window(LPCWSTR wnd_title, LPCWSTR wnd_class, int width, int height, int pos_x, int pos_y);
		
		// Description
		// When out of scope. This deconstructor calls UnregisterClass() and DestroyWindow()
		// to remove the window from memory.
		~Window();

		// Copy/Copy-Assign Operator
		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;

		// Description
		// This function renders the window using ShowWindow().
		// It also uses a parameter that can change how the window is rendered.
		void Render(int nCmdShow);

		// Description
		// Uses PeekMessage to look at the msg without stopping the program
		// and translate/dispatches the message.
		bool ProcessMessages();

		// Getter
		HWND GetWindowHandle();
		
	private:
		// Description
		// This function creates a Window Class Extension
		void RegisterWindowClass();

		// Description
		// When Window Class Extension is being contructed and WM_NCCREATE has been sent.
		// This function will setup how the window will handle messages by creating a pointer
		// to the Engine class's WndProc function.
		static LRESULT CALLBACK SetUpMsgHandle(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

		// Description
		// When a messages comes to the window class, this function will redirect that message
		// to a WndProc function inside another class like engine.
		static LRESULT CALLBACK RedirectMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

		int wnd_width	 = 0;
		int wnd_height	 = 0;
		int wnd_posX	 = 0;
		int wnd_posY	 = 0;

		HWND handle = NULL; // Window Handler
		HINSTANCE hInstance = NULL; // Handler to instance of Window application

		LPCWSTR window_title = L"WindowTitle";
		LPCWSTR window_class_name = L"WindowClass";
};
