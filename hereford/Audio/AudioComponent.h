#pragma once
#include "Core/Component.h"
#include <string>
#include "AudioManager.h"

class AudioComponent : public Component
{
public:
	AudioComponent(class Actor* owner, class AudioManager& audioManagerRef, bool is3D = false);
	~AudioComponent();

	void InitAsset(const std::string& assetName);

	void Play(bool loop = false);
	void Pause();
	void Stop();
	ESoundState GetSoundState() const { return mAudioManager.GetSoundState(mHandle); }

private:
	class AudioManager& mAudioManager;
	std::string mSoundAssetName;
	SoundHandle mHandle;
	bool mIs3D;
};

