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
	AudioManager(int channelNum = 16);
	~AudioManager();

	void Update();

	SoundHandle PlaySound(const std::string& soundName, bool shouldLooping = false, bool is3D = false, Vec3 soundPos = Vec3::Zero);

	void StopSound(SoundHandle sound, bool fadeOut = false, float fadeOutDuration = 0.0f);
	void PauseSound(SoundHandle sound, bool fadeOut = false, float fadeOutDuration = 0.0f);
	void ResumeSound(SoundHandle sound, bool fadeIn = false, float fadeInDuration = 0.0f);
	ESoundState GetSoundState(SoundHandle sound);
	void StopAllSounds();
	void CacheAllSounds();
	void CacheSound(const std::string& soundName);

	void SetPlayerReference(class Actor* player) { mPlayerRef = player; };

private:
	struct HandleInfo
	{
		std::string mSoundName;
		uint8 mChannel = -1;
		bool mIsLooping = false;
		bool mIsPaused = false;
		bool mIs3D = false;
		Vec3 mSoundPos = Vec3::Zero;
	};

	struct Mix_Chunk* GetSound(const std::string& soundName);
	void Set3DEffect(int channelNum, Vec3 soundPos);
	void AdjustVolumeTimeline(int channelNum, int startVolume, int targetVolume, float alpha);
	void Pause(SoundHandle sound);
	void Stop(SoundHandle sound);

	

	std::vector<SoundHandle> mChannels;
	std::map<SoundHandle, HandleInfo> mHandleMap;
	std::unordered_map<std::string, struct Mix_Chunk*> mSoundAssets;

	class Actor* mPlayerRef;

	SoundHandle mLastHandle = 0;

};

