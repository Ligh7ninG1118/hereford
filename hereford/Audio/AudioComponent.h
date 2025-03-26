#pragma once
#include "Core/OldComponent.h"
#include "HerefordCommon.h"

#include "AudioSystem.h"



class AudioComponent : public Component
{
public:
	AudioComponent(class Actor* owner, class AudioManager& audioManagerRef, bool is3D = false);
	~AudioComponent();

	void InitAsset(const std::string& assetName);

private:
	friend class AudioSystem;

	EAudioState mCurrentState : 2;
	EAudioState mPendingState : 2;

	bool mShouldLoop :1;
	bool mShouldFade :1;
	bool mIs3D :1;
	float mFadeDuration;

	std::string mSoundAssetName;
	SoundHandle mHandle;
	
};

