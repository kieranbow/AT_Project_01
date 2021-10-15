#pragma once
// Windows
#include <Windows.h>

// C++
#include <string>

struct StructWndClass
{
	UINT			cbSize;
	UINT			style;
	WNDPROC   lpfnWndProc;
	int				cbClsExtra;
	int				cbWndExtra;
	HINSTANCE hInstance;
	HICON		hIcon;
	HCURSOR   hCursor;
	HBRUSH		hbrBackground;
	LPCWSTR	lpszMenuName;
	LPCWSTR	lpszClassName;
	HICON		hIconSm;
};

struct V2Int
{
	V2Int(int _x, int _y) : x(_x), y(_y) {}
	int x;
	int y;
};

class Window
{
	public:
		// Constructor & Destructor
		Window(HINSTANCE hInst, LPCWSTR wnd_title, LPCWSTR wnd_class, V2Int size, V2Int position);
		~Window();

		// Copy/Copy-Assign Operator
		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;

		void Render(int nCmdShow);
		bool ProcessMessages();
		
	private:
		int width		= 0;
		int height	= 0;
		int pos_x	= 0;
		int pos_y	= 0;

		HWND handle				= NULL; // Window Handler
		HINSTANCE hInstance	= NULL; // Handler to instance of Window application

		LPCWSTR window_title				= L"Default";
		LPCWSTR window_class_name	= L"Default";
};
