#pragma once
// Windows
#include <Windows.h>

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

class Window
{
	public:
		// Constructor & Destructor
		Window(LPCWSTR wnd_title, LPCWSTR wnd_class, int width, int height, int pos_x, int pos_y);
		~Window();

		// Copy/Copy-Assign Operator
		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;

		void Render(int nCmdShow);
		bool ProcessMessages();
		
	private:
		HWND handle				= NULL; // Window Handler
		HINSTANCE hInstance	= NULL; // Handler to instance of Window application

		LPCWSTR window_title				= L"WindowTitle";
		LPCWSTR window_class_name	= L"WindowClass";

		void RegisterWindowClass();

		static LRESULT CALLBACK SetUpMsgHandle(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK RedirectMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

		int wnd_width	 = 0;
		int wnd_height	 = 0;
		int wnd_posX	 = 0;
		int wnd_posY	 = 0;
};
