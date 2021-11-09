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
	Engine bowEngine(L"funny box", L"MyWindow", 1280, 720, 50, 50);

	Timer timer;

	// Process window messages
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
