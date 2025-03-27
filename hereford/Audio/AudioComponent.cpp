#include "AudioComponent.h"
#include "Audio/AudioManager.h"
#include "Core/Actor.h"

AudioComponent::AudioComponent(Actor* owner, AudioManager& audioManagerRef, bool is3D)
	: OldComponent(owner),
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

void AudioComponent::Resume(bool fadeIn, bool fadeInDuration)
{
	if (mHandle == 0)
	{
		printf("AudioComponent::Resume(): Audio Component's handle is not assigned to any Sound Handle\n");
		return;
	}

	mAudioManager.ResumeSound(mHandle, fadeIn, fadeInDuration);
}

void AudioComponent::Pause(bool fadeOut, bool fadeOutDuration)
{
	if (mHandle == 0)
	{
		printf("AudioComponent::Pause(): Audio Component's handle is not assigned to any Sound Handle\n");
		return;
	}

	mAudioManager.PauseSound(mHandle, fadeOut, fadeOutDuration);
}

void AudioComponent::Stop(bool fadeOut, bool fadeOutDuration)
{
	if (mHandle == 0)
	{
		printf("AudioComponent::Stop(): Audio Component's handle is not assigned to any Sound Handle\n");
		return;
	}

	mAudioManager.StopSound(mHandle, fadeOut, fadeOutDuration);
	mHandle = 0;
}

