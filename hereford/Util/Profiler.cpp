#include <algorithm>
#include "Profiler.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl2.h"
#include "imgui/imgui_impl_opengl3.h"

std::unordered_map<std::string, WatchData> Profiler::mWatchMap;
time_unit_t Profiler::mSelfUpdateTimer = steady_clock::now();
time_unit_t Profiler::mFPSTimer = steady_clock::now();
float Profiler::mUpdatePeriod = 1.0f;
float Profiler::mTotalFrameTime = 0.0f;

void Profiler::UpdateImGuiView()
{
	ImGui::Begin("Telemetry", 0, ImGuiWindowFlags_AlwaysAutoResize);

	if(ShouldUpdate())
		mTotalFrameTime = duration<float>(steady_clock::now() - mFPSTimer).count();
	mFPSTimer = steady_clock::now();
	ImGui::Text("FPS: %.1f\t\t\tTotal Frame Time: %.2f ms\n", 1.0f / mTotalFrameTime, mTotalFrameTime * 1000.0f);

	// Sort using display order
	std::vector<std::pair<std::string, WatchData>> watchVec(mWatchMap.begin(), mWatchMap.end());
	std::sort(watchVec.begin(), watchVec.end(), [](const auto& a, const auto& b) {
		return a.second.mDisplayOrder < b.second.mDisplayOrder;
		});

	for (auto& itr : watchVec)
	{
		ImGui::Text("%-20s: Avg %6.2f ms\tCurrent %6.2f ms \n", itr.first.c_str(), itr.second.mTotalTaskTime * 1000.0f / itr.second.mTaskRep, itr.second.mLastDuration * 1000.0f);
	}
	ImGui::End();

	if (ShouldUpdate())
		mSelfUpdateTimer = steady_clock::now();
}

void Profiler::Start(std::string watchName, unsigned int displayOrder)
{
	auto itr = mWatchMap.find(watchName);
	if (itr != mWatchMap.end())
	{
		mWatchMap[watchName].mTimeStamp = steady_clock::now();
	}
	else
	{
		WatchData newData{ displayOrder, steady_clock::now() , 0.0f, 0.0f, 0 };
		mWatchMap[watchName] = newData;
	}
}

float Profiler::Peek(std::string watchName) 
{
	auto itr = mWatchMap.find(watchName);
	if (itr != mWatchMap.end())
	{
		return duration<float>(steady_clock::now() - mWatchMap[watchName].mTimeStamp).count();
	}
	else
	{
		//TODO: Log error message
		return 0.0f;
	}
}

void Profiler::Mark(std::string watchName)
{
	auto itr = mWatchMap.find(watchName);
	if (itr != mWatchMap.end())
	{
		// Only calculate duration value when view is updating
		if(ShouldUpdate())
			mWatchMap[watchName].mLastDuration = duration<float>(steady_clock::now() - mWatchMap[watchName].mTimeStamp).count();
		mWatchMap[watchName].mTimeStamp = steady_clock::now();
		mWatchMap[watchName].mTotalTaskTime += mWatchMap[watchName].mLastDuration;
		mWatchMap[watchName].mTaskRep++;
	}
	else
	{
		//TODO: Log error message
	}
}

// Controls view updating frequency
bool Profiler::ShouldUpdate()
{
	return duration<float>(steady_clock::now() - mSelfUpdateTimer).count() >= mUpdatePeriod ? true : false;
}
