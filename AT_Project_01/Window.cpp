#include "Window.h"
#include "Engine.h"

Window::Window(LPCWSTR wnd_title, LPCWSTR wnd_class, int width, int height, int pos_x, int pos_y)
{
	// Set Base Window Propertise
	hInstance				= GetModuleHandle(NULL);
	window_title			= wnd_title;
	window_class_name	= wnd_class;
	wnd_width				= width;
	wnd_height				= height;
	wnd_posX				= pos_x;
	wnd_posY				= pos_y;

	// Adjust window size to fit client region size
	RECT wndRect;
	wndRect.left = 100;
	wndRect.right = width + wndRect.left;
	wndRect.top = 100;
	wndRect.bottom = height + wndRect.top;
	AdjustWindowRect(&wndRect, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);

	RegisterWindowClass();

	// Create window
	handle = CreateWindowEx(
		0,
		window_class_name,
		window_title,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		pos_x,
		pos_y,
		wndRect.right - wndRect.left,
		wndRect.bottom - wndRect.top,
		NULL,
		NULL,
		hInstance,
		this);

	if (!handle)
	{
		int nResult = GetLastError();
		MessageBox(NULL, TEXT("Window creation failed"), TEXT("Window Failed"), MB_ICONERROR);
	}

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
		if (!IsWindow(handle))
		{
			handle = NULL;
			UnregisterClass(window_class_name, hInstance);
			return false;
		}
	}

	return true;
}

void Window::RegisterWindowClass()
{
	// Create Window Class Extenstion
	WNDCLASSEX wc = { 0 };
	ZeroMemory(&wc, sizeof(wc));
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = SetUpMsgHandle;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = NULL;
	wc.hCursor = NULL;
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = window_class_name;
	wc.hIconSm = NULL;
	wc.cbSize = sizeof(wc);

	// Register Window Class Extention
	if (!RegisterClassEx(&wc))
	{
		int nResult = GetLastError();
		MessageBox(NULL, TEXT("Window class creation failed"), TEXT("Window Class Failed"), MB_ICONERROR);
	}
}

// ---------------Messages-------------------
//
LRESULT CALLBACK Window::SetUpMsgHandle(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_NCCREATE) // NCCREATE = Non-client Create
	{
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Engine* const pWindow = reinterpret_cast<Engine*>(pCreate->lpCreateParams);

		if (pWindow == NULL)
		{
			// Error logger goes here
			exit(-1);
		}

		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow));
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::RedirectMsg));
		// return pWindow->HandleMsg(hWnd, msg, wParam, lParam);
		return pWindow->WndProc(hWnd, msg, wParam, lParam);
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);

}

LRESULT CALLBACK Window::RedirectMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_CLOSE)
	{
		DestroyWindow(hWnd);
		return 0;
	}

	// Gets the long pointer from the window and passes msg info to HandleMsg function
	Engine* const pWindow = reinterpret_cast<Engine*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	
	return pWindow->WndProc(hWnd, msg, wParam, lParam);
	
	// return pWindow->HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		if (wParam == 'D')
		{
			SetWindowText(hWnd, L"New Text");
		}
		break;

	case WM_KEYUP:
		if (wParam == 'F')
		{
			SetWindowText(hWnd, L"More Text");
		}
		break;

	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
