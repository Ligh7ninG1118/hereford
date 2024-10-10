#pragma once
#include "Core/Component.h"
#include <string>

class AudioComponent : public Component
{
public:
	AudioComponent(class Actor* owner, class AudioManager& audioManagerRef, bool is3D = false);
	~AudioComponent();

	void InitAsset(const std::string& assetName);

	void Play(bool loop = false);

private:
	class AudioManager& mAudioManager;
	std::string mSoundAssetName;
	bool mIs3D;
};

