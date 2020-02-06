#include <chrono>
#include "timeutils.h"


static std::chrono::steady_clock::time_point _previousTimePoint = std::chrono::steady_clock::now();

double getTimeDeltaSeconds() {

	std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
	long long timeDelta = std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - _previousTimePoint).count();
	double timeDeltaSeconds = (double)timeDelta / 1000000000.0;
	_previousTimePoint = currentTime;
	return timeDeltaSeconds;
}