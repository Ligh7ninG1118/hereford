#include "AudioComponent.h"
#include "Core/Actor.h"

AudioComponent::AudioComponent(uint32_t ownerID, const std::string& assetName)
	: Component(ownerID),
	mSoundAssetName(assetName)
{
}

AudioComponent::~AudioComponent()
{
}


