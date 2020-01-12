#ifndef UTILS_HPP
#define UTILS_HPP

#include <chrono>
#include <thread>

template<typename resolution, int targetFps>
int limitFps()
{
	using ClockType = std::conditional<
		std::chrono::high_resolution_clock::is_steady,
		std::chrono::high_resolution_clock,
		std::chrono::steady_clock>::type;

	static auto startTime = ClockType::now();
	static auto endTime = ClockType::now();

	// Calculate the target frame time based on the target fps
	constexpr std::chrono::seconds sec(1);
	constexpr int targetFrameTime = resolution(sec).count() / targetFps;

	// Sample first time point
	startTime = ClockType::now();

	// Get the time spent not sleeping this frame
	int workTime = std::chrono::duration_cast<resolution>(startTime - endTime).count();

	// Sleep the process if needed
	if(workTime < targetFrameTime)
		std::this_thread::sleep_for(resolution(targetFrameTime - workTime));

	// Sample second time point
	endTime = ClockType::now();

	// Get the time spent sleeping this frame
	int sleepTime = std::chrono::duration_cast<resolution>(endTime - startTime).count();

	// The total number of ticks this frame is: sleep time + non sleep time
	int ticks = sleepTime + workTime;
	return ticks;
}

#endif
