#include "AudioComponent.h"
#include "Audio/AudioManager.h"
#include "Core/Actor.h"

AudioComponent::AudioComponent(Actor* owner, AudioManager& audioManagerRef, bool is3D)
	: Component(owner),
	mAudioManager(audioManagerRef),
	mIs3D(is3D),
	mHandle(0)
{
}

AudioComponent::~AudioComponent()
{
}

void AudioComponent::InitAsset(const std::string& assetName)
{
	mAudioManager.CacheSound(assetName);
	mSoundAssetName = assetName;
}

void AudioComponent::Play(bool loop)
{
	mHandle = mAudioManager.PlaySound(mSoundAssetName, loop, mIs3D, mOwner->GetPosition());
}

void AudioComponent::Pause()
{
	if (mHandle == 0)
	{
		printf("AudioComponent::Pause(): Audio Component's handle is not assigned to any Sound Handle\n");
		return;
	}

	mAudioManager.PauseSound(mHandle);
}

void AudioComponent::Stop()
{
	if (mHandle == 0)
	{
		printf("AudioComponent::Stop(): Audio Component's handle is not assigned to any Sound Handle\n");
		return;
	}

	mAudioManager.StopSound(mHandle);
	mHandle = 0;
}

