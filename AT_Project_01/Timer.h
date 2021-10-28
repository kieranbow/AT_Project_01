#pragma once
#include <chrono>

class Timer
{
	public:
		Timer();

		double GetMilisecondsElapse();
		void Restart();
		bool Stop();
		bool Start();

	private:
		bool isRunning = false;
		std::chrono::time_point<std::chrono::steady_clock> start;
		std::chrono::time_point<std::chrono::steady_clock> stop;
};
