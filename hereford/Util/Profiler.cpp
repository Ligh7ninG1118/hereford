#include "Profiler.h"

std::unordered_map<std::string, time_unit_t> Profiler::mWatchMap;

void Profiler::Init(std::string watchName)
{
	auto itr = mWatchMap.find(watchName);
	if (itr == mWatchMap.end())
	{
		mWatchMap[watchName] = std::chrono::steady_clock::now();
	}
	else
	{
		//Log warning message
	}
}

float Profiler::Peek(std::string watchName)
{
	auto itr = mWatchMap.find(watchName);
	if (itr != mWatchMap.end())
	{
		return duration<float>(steady_clock::now() - mWatchMap[watchName]).count();
	}
	else
	{
		//Log error message
		return 0.0f;
	}
}

float Profiler::Update(std::string watchName)
{
	auto itr = mWatchMap.find(watchName);
	if (itr != mWatchMap.end())
	{
		float result = duration<float>(steady_clock::now() - mWatchMap[watchName]).count();
		mWatchMap[watchName] = steady_clock::now();
		return result;
	}
	else
	{
		//Log error message
		return 0.0f;
	}
}
