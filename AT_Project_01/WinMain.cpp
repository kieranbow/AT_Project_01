#include "Engine.h"
#include "Timer.h"

// Main entry point for WinAPI
int CALLBACK WinMain(
	_In_ HINSTANCE hInstance, 
	_In_opt_ HINSTANCE hPrevInstance, 
	_In_ LPSTR lpCmdLine, 
	_In_ int nCmdShow)
{
	// Creates engine instance
	Engine bowEngine(L"First Person Game", L"MyWindow", 1280, 720, 50, 50);

	Timer timer;

	while (bowEngine.ProcessWndMessages() == true)
	{
		double dt = timer.GetMilisecondsElapse();
		timer.Restart();

		bowEngine.Input();
		bowEngine.Update(dt);
		bowEngine.RenderFrame();
	}

	return 0;
}
