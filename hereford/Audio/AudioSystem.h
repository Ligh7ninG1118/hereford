#pragma once

#include "HerefordCommon.h"

#include "Audio/AudioComponent.h"

using SoundHandle = uint32_t;

enum class EAudioState : uint8_t
{
	Stopped = 0,
	Paused,
	Playing
};

class AudioSystem : public System
{
public:
	AudioSystem(int channelNum = 16);
	~AudioSystem();

	virtual void Update(float deltaTime) override;

	SoundHandle PlaySound(const std::string& soundName, bool shouldLooping = false, bool is3D = false, Vec3 soundPos = Vec3::Zero);
	bool StopSound(SoundHandle sound, bool fadeOut = false, float fadeOutDuration = 0.0f);
	bool PauseSound(SoundHandle sound, bool fadeOut = false, float fadeOutDuration = 0.0f);
	bool ResumeSound(SoundHandle sound, bool fadeIn = false, float fadeInDuration = 0.0f);

	EAudioState GetSoundState(SoundHandle sound);
	void StopAllSounds();
	void CacheAllSounds();
	void CacheSound(const std::string& soundName, int chuckVolumeOverride = 128);

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

	void UpdateComponentStates();

	struct Mix_Chunk* GetSound(const std::string& soundName, int chuckVolumeOverride = 128);
	void Set3DEffect(int channelNum, Vec3 soundPos);
	void AdjustVolumeTimeline(int channelNum, int startVolume, int targetVolume, float alpha);
	void Pause(SoundHandle sound);
	void Stop(SoundHandle sound);


	std::vector<AudioComponent*> mAudioComponents;
	std::vector<SoundHandle> mChannels;
	std::map<SoundHandle, HandleInfo> mHandleMap;
	std::unordered_map<std::string, struct Mix_Chunk*> mSoundAssets;

	SoundHandle mLastHandle = 0;
};

