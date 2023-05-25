#pragma once
#ifndef TIMER_H
#define TIMER_H

#include <iostream>
#include <chrono>

class Timer
{
public:
	Timer()
	{
		mStartTime = std::chrono::high_resolution_clock::now();
	}

	~Timer()
	{
		Stop();
	}

	void Stop()
	{
		auto endTime = std::chrono::high_resolution_clock::now();

		auto start = std::chrono::time_point_cast<std::chrono::microseconds>(mStartTime).time_since_epoch().count();
		auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTime).time_since_epoch().count();

		auto duration = end - start;

		double ms = duration * 0.001;

		std::cout << ms << " milliseconds (" << duration << " microseconds)\n";
	}

private:
	std::chrono::time_point< std::chrono::high_resolution_clock> mStartTime;
};

#endif // !TIMER_H
