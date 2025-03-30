#pragma once

#include "HerefordCommon.h"


using SoundHandle = uint32_t;

class AudioComponent;
class ListenerComponent;

enum class EAudioState
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

	SoundHandle PlaySound(AudioComponent* audioComp);
	bool StopSound(AudioComponent* audioComp);
	bool PauseSound(AudioComponent* audioComp);
	bool ResumeSound(AudioComponent* audioComp);

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


	ListenerComponent* mMainListener;
	std::vector<AudioComponent*> mAudioComponents;
	
	std::vector<SoundHandle> mChannels;
	std::map<SoundHandle, HandleInfo> mHandleMap;
	std::unordered_map<std::string, struct Mix_Chunk*> mSoundAssets;

	SoundHandle mLastHandle = 0;
};

