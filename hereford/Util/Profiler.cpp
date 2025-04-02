#include "Profiler.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl2.h"
#include "imgui/imgui_impl_opengl3.h"

std::unordered_map<std::string, WatchData> Profiler::mWatchMap;
time_unit_t Profiler::mSelfUpdateTimer = steady_clock::now();
float Profiler::mUpdatePeriod = 0.5f;


void Profiler::UpdateImGuiView()
{
	bool shouldUpdate = duration<float>(steady_clock::now() - mSelfUpdateTimer).count() >= mUpdatePeriod ? true : false;
	if (shouldUpdate)
		mSelfUpdateTimer = steady_clock::now();

	ImGui::Begin("Telemetry");
	for (auto& itr : mWatchMap)
	{
		if(shouldUpdate)
			itr.second.mDisplayDuration = itr.second.mLastDuration;

		ImGui::Text("%s: %.2f ms\n", itr.first.c_str(), itr.second.mDisplayDuration * 1000.0f);
	}
	ImGui::End();
}

void Profiler::Start(std::string watchName)
{
	auto itr = mWatchMap.find(watchName);
	if (itr != mWatchMap.end())
	{
		mWatchMap[watchName].mTimeStamp = steady_clock::now();
	}
	else
	{
		WatchData newData{ steady_clock::now() , 0.0f, 0.0f };
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
		//Log error message
		return 0.0f;
	}
}

void Profiler::Mark(std::string watchName)
{
	auto itr = mWatchMap.find(watchName);
	if (itr != mWatchMap.end())
	{
		mWatchMap[watchName].mLastDuration = duration<float>(steady_clock::now() - mWatchMap[watchName].mTimeStamp).count();
		mWatchMap[watchName].mTimeStamp = steady_clock::now();
	}
	else
	{
		//Log error message
	}
}
