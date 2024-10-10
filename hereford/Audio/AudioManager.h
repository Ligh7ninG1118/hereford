#pragma once
#include <vector>
#include <map>
#include <string>
#include <unordered_map>
#include "Math/Math.h"


using SoundHandle = uint32_t;

enum class ESoundState
{
	Stopped = 0,
	Playing,
	Paused
};

class AudioManager
{
public:
	AudioManager(int channelNum = 8);
	~AudioManager();

	void Update();

	SoundHandle PlaySound(const std::string& soundName, bool shouldLooping = false);

	void StopSound(SoundHandle sound);
	void PauseSound(SoundHandle sound);
	void ResumeSound(SoundHandle sound);
	ESoundState GetSoundState(SoundHandle sound);
	void StopAllSounds();
	void CacheAllSounds();
	void CacheSound(const std::string& soundName);

private:
	struct Mix_Chunk* GetSound(const std::string& soundName);

	struct HandleInfo
	{
		std::string mSoundName;
		uint8 mChannel = -1;
		bool mIsLooping = false;
		bool mIsPaused = false;
	};

	std::vector<SoundHandle> mChannels;
	std::map<SoundHandle, HandleInfo> mHandleMap;
	std::unordered_map<std::string, struct Mix_Chunk*> mSounds;

	SoundHandle mLastHandle = 0;

};

