#pragma once
#include "HerefordCommon.h"

#include "AudioSystem.h"



class AudioComponent : public Component
{
public:
	AudioComponent(uint32_t ownerID, const std::string& assetName);
	~AudioComponent();

	void Play() { SetState(EAudioState::Playing); }
	void Pause() { SetState(EAudioState::Paused); }
	void Stop() { SetState(EAudioState::Stopped); }

	void SetShouldLoop(bool inVal) { mShouldLoop = inVal; }
	void SetShouldFade(bool inVal) { mShouldFade = inVal; }
	void SetIs3D(bool inVal) { mIs3D = true; }
	void SetFadeDuration(float inDuration) { mFadeDuration = inDuration; }

private:
	void SetState(EAudioState inState) { mPendingState = inState; }

	friend class AudioSystem;

	EAudioState mCurrentState;
	EAudioState mPendingState;

	bool mShouldLoop :1;
	bool mShouldFade :1;
	bool mIs3D :1;
	float mFadeDuration;

	std::string mSoundAssetName;
	SoundHandle mHandle;
	
};

