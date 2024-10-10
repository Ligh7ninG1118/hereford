#include "AudioComponent.h"
#include "Audio/AudioManager.h"
#include "Core/Actor.h"

AudioComponent::AudioComponent(Actor* owner, AudioManager& audioManagerRef, bool is3D)
	: Component(owner),
	mAudioManager(audioManagerRef),
	mIs3D(is3D)
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
	mAudioManager.PlaySound(mSoundAssetName, loop, mIs3D, mOwner->GetPosition());
}
