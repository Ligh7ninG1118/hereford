#pragma once
#include <unordered_map>
#include <string>
#include <chrono>

using namespace std::chrono;
using time_unit_t = std::chrono::time_point<std::chrono::steady_clock>;

struct WatchData
{
	time_unit_t mTimeStamp;
	float mLastDuration;
	float mDisplayDuration;
};

class Profiler
{
public:
	static void UpdateImGuiView();

	static void Start(std::string watchName);

	static float Peek(std::string watchName);

	static void Mark(std::string watchName);

private:
	static float mUpdatePeriod;
	static time_unit_t mSelfUpdateTimer;
	static std::unordered_map<std::string, WatchData> mWatchMap;
};

