#pragma once
#include <unordered_map>
#include <string>
#include <chrono>

using namespace std::chrono;
using time_unit_t = std::chrono::time_point<std::chrono::steady_clock>;

struct WatchData
{
	unsigned int mDisplayOrder;
	time_unit_t mTimeStamp;
	float mLastDuration;
	float mTotalTaskTime;
	uint32_t mTaskRep;
};

class Profiler
{
public:
	static void UpdateImGuiView();

	static void Start(std::string watchName, unsigned int displayOrder = 100);

	static float Peek(std::string watchName);

	static void Mark(std::string watchName);

private:
	inline static bool ShouldUpdate();

	static float mUpdatePeriod;
	static float mTotalFrameTime;
	static time_unit_t mSelfUpdateTimer;
	static time_unit_t mFPSTimer;
	static std::unordered_map<std::string, WatchData> mWatchMap;
};

