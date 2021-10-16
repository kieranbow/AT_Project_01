#include "Window.h"

// Main entry point for WinAPI
int CALLBACK WinMain(
	_In_ HINSTANCE hInstance, 
	_In_opt_ HINSTANCE hPrevInstance, 
	_In_ LPSTR lpCmdLine, 
	_In_ int nCmdShow)
{
	// Creates window instance
	Window window(hInstance, L"funny box", L"MyWindow", 800, 600, 200, 200);

	// Render Window
	window.Render(SW_SHOWDEFAULT);

	// Process window messages
	while (window.ProcessMessages()== true)
	{

	}
	return 0;
}
