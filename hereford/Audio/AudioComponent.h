#pragma once
#include "Core/OldComponent.h"
#include <string>
#include "AudioManager.h"

class AudioComponent : public OldComponent
{
public:
	AudioComponent(class Actor* owner, class AudioManager& audioManagerRef, bool is3D = false);
	~AudioComponent();

	void InitAsset(const std::string& assetName);

	void Play(bool loop = false);
	void Resume(bool fadeIn = false, bool fadeInDuration = 0.0f);
	void Pause(bool fadeOut = false, bool fadeOutDuration = 0.0f);
	void Stop(bool fadeOut = false, bool fadeOutDuration = 0.0f);
	ESoundState GetSoundState() const { return mAudioManager.GetSoundState(mHandle); }

private:
	class AudioManager& mAudioManager;
	std::string mSoundAssetName;
	SoundHandle mHandle;
	bool mIs3D;
};

