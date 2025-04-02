#pragma once
#include <unordered_map>
#include <string>
#include <chrono>

using namespace std::chrono;
using time_unit_t = std::chrono::time_point<std::chrono::steady_clock>;

class Profiler
{
public:
	static void Init(std::string watchName);

	static float Peek(std::string watchName);

	static float Update(std::string watchName);

private:
	static std::unordered_map<std::string, time_unit_t> mWatchMap;
};

