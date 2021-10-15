#include "Window.h"

Window::Window(HINSTANCE hInst, LPCWSTR wnd_title, LPCWSTR wnd_class, V2Int size, V2Int position)
{
	// Set Base Window Propertice
	hInstance					= hInst;
	window_title				= wnd_title;
	window_class_name		= wnd_class;
	width							= size.x;
	height						= size.y;
	pos_x							= position.x;
	pos_y							= position.y;

	// Create Window Class Extenstion
	WNDCLASSEX wc = { 0 };
	ZeroMemory(&wc, sizeof(wc));
	wc.style					= CS_OWNDC;
	wc.lpfnWndProc		= DefWindowProc;
	wc.cbClsExtra			= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance			= hInstance;
	wc.hIcon				= NULL;
	wc.hCursor				= NULL;
	wc.hbrBackground	= NULL;
	wc.lpszMenuName	= NULL;
	wc.lpszClassName		= window_class_name;
	wc.hIconSm			= NULL;
	wc.cbSize				= sizeof(wc);

	// Register Window Class Extention
	RegisterClassEx(&wc);

	// Creates window
	handle = CreateWindowEx(
		0,
		window_class_name,
		window_title,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		pos_x,
		pos_y,
		width,
		height,
		NULL,
		NULL,
		hInstance,
		nullptr);
}

Window::~Window()
{
	UnregisterClass(window_title, hInstance);
	DestroyWindow(handle);
}

void Window::Render(int nCmdShow)
{
	// Render Window
	ShowWindow(handle, nCmdShow);
}

bool Window::ProcessMessages()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	if (PeekMessage(&msg, handle, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if (msg.message == WM_NULL)
	{
		if (!IsWindow(this->handle))
		{
			this->handle = NULL;
			UnregisterClass(window_class_name, hInstance);
			return false;
		}
	}

	return true;
}
