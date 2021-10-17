#include "Engine.h"

// Main entry point for WinAPI
int CALLBACK WinMain(
	_In_ HINSTANCE hInstance, 
	_In_opt_ HINSTANCE hPrevInstance, 
	_In_ LPSTR lpCmdLine, 
	_In_ int nCmdShow)
{
	// Creates engine instance
	Engine bowEngine(L"funny box", L"MyWindow", 800, 600, 200, 200);

	// Process window messages
	while (bowEngine.ProcessWndMessages()== true)
	{
		bowEngine.Update();
	}

	return 0;
}
