#include "AudioSystem.h"
#include "AudioComponent.h"
#include "ListenerComponent.h"
#include "Core/Actor.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"
#include "imgui/imgui.h"
#include "Util/TimelineAction.h"


AudioSystem::AudioSystem(int channelNum)
{
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	Mix_AllocateChannels(channelNum);
	mChannels.resize(channelNum);
}

AudioSystem::~AudioSystem()
{
	for (auto& s : mSoundAssets)
	{
		Mix_FreeChunk(s.second);
	}
	mSoundAssets.clear();
	Mix_CloseAudio();
}

void AudioSystem::Update(float deltaTime)
{
	UpdateComponentStates();

	for (int i = 0; i < mChannels.size(); i++)
	{
		if (mChannels[i] != 0 && !Mix_Playing(i))
		{
			mHandleMap.erase(mChannels[i]);
			mChannels[i] = 0;
		}
		//TODO: need a way to distinguish channel playing 3d sound
		else if (Mix_Playing(i))
		{
			if (mHandleMap[mChannels[i]].mIs3D)
				Set3DEffect(i, mHandleMap[mChannels[i]].mSoundPos);
		}
	}
}

void AudioSystem::UpdateComponentStates()
{
	for (AudioComponent* audioComp : mAudioComponents)
	{
		EAudioState pendingState = static_cast<EAudioState>(audioComp->mPendingState);
		if (pendingState == static_cast<EAudioState>(audioComp->mCurrentState))
			continue;

		//TODO: This will cause state change and invalidate fade in/out immediately?

		switch (pendingState)
		{
		case EAudioState::Stopped:
			if (StopSound(audioComp))
			{
				audioComp->mCurrentState = EAudioState::Stopped;
			}
			break;
		case EAudioState::Paused:
			if (PauseSound(audioComp))
			{
				audioComp->mCurrentState = EAudioState::Paused;
			}
			break;
		case EAudioState::Playing:
			if (PlaySound(audioComp))
			{
				audioComp->mCurrentState = EAudioState::Playing;
			}
			break;
		default:
			break;
		}
	}
}

SoundHandle AudioSystem::PlaySound(AudioComponent* audioComp)
{
	Mix_Chunk* pChunk = GetSound(audioComp->mSoundAssetName);
	if (pChunk == nullptr)
	{
		printf("AudioManager::PlaySound(): Unable to get sound asset named [%s]\n", audioComp->mSoundAssetName.c_str());
		return 0;
	}

	for (int i = 0; i < mChannels.size(); i++)
	{
		if (mChannels[i] == 0)
		{
			mLastHandle++;
			HandleInfo handle(audioComp->mSoundAssetName, i, audioComp->mShouldLoop, false, audioComp->mIs3D);
			mHandleMap[mLastHandle] = handle;
			mChannels[i] = mLastHandle;
			Mix_Volume(i, 128);
			Mix_PlayChannel(i, pChunk, audioComp->mShouldLoop ? -1 : 0);

			break;
		}
	}

	//TODO: Prioritization

	/*The oldest instance of the same soundName and if there is none�
		The oldest non - looping sound and if there is none�
		The oldest sound*/


	return mLastHandle;
}

bool AudioSystem::StopSound(AudioComponent* audioComp)
{
	auto itr = mHandleMap.find(audioComp->mHandle);
	if (itr == mHandleMap.end())
	{
		printf("AudioManager::StopSound(): Unable to find sound handle of id [%d]\n", audioComp->mHandle);
		return false;
	}

	if (audioComp->mShouldFade)
	{
		TAHandle temp;
		TimelineActionManager::PlayFromStart(temp,
			std::bind(&AudioSystem::AdjustVolumeTimeline, this, itr->second.mChannel, Mix_Volume(itr->second.mChannel, -1), 0, std::placeholders::_1),
			audioComp->mFadeDuration,
			std::bind(&AudioSystem::Stop, this, audioComp->mHandle));
	}
	else
	{
		Stop(audioComp->mHandle);
	}


	return true;
}

bool AudioSystem::PauseSound(AudioComponent* audioComp)
{
	auto itr = mHandleMap.find(audioComp->mHandle);
	if (itr == mHandleMap.end())
	{
		printf("AudioManager::PauseSound(): Unable to find sound handle of id [%d]\n", audioComp->mHandle);
		return false;
	}

	if (!itr->second.mIsPaused)
	{
		if (audioComp->mShouldFade)
		{
			TAHandle temp;
			TimelineActionManager::PlayFromStart(temp,
				std::bind(&AudioSystem::AdjustVolumeTimeline, this, itr->second.mChannel, Mix_Volume(itr->second.mChannel, -1), 0, std::placeholders::_1),
				audioComp->mFadeDuration,
				std::bind(&AudioSystem::Pause, this, audioComp->mHandle));
		}
		else
		{
			Pause(audioComp->mHandle);
		}
	}

	return true;
}

bool AudioSystem::ResumeSound(AudioComponent* audioComp)
{
	auto itr = mHandleMap.find(audioComp->mHandle);
	if (itr == mHandleMap.end())
	{
		printf("AudioManager::ResumeSound(): Unable to find sound handle of id [%d]\n", audioComp->mHandle);
		return false;
	}

	if (itr->second.mIsPaused)
	{
		Mix_Resume(itr->second.mChannel);
		itr->second.mIsPaused = false;

		if (audioComp->mShouldFade)
		{
			TAHandle temp;
			int channelVolume = Mix_Volume(itr->second.mChannel, -1);
			int targetVolume = channelVolume == 0 ? 128 : channelVolume;

			TimelineActionManager::PlayFromStart(temp,
				std::bind(&AudioSystem::AdjustVolumeTimeline, this, itr->second.mChannel, 0, targetVolume, std::placeholders::_1),
				audioComp->mFadeDuration);
		}
	}

	return true;
}

EAudioState AudioSystem::GetSoundState(SoundHandle sound)
{
	auto itr = mHandleMap.find(sound);
	if (itr == mHandleMap.end())
		return EAudioState::Stopped;

	return itr->second.mIsPaused ? EAudioState::Paused : EAudioState::Playing;
}

void AudioSystem::StopAllSounds()
{
	Mix_HaltChannel(-1);
	for (auto& c : mChannels)
	{
		c = 0;
	}
	mHandleMap.clear();
}

void AudioSystem::CacheAllSounds()
{
	for (auto* comp : mAudioComponents)
	{
		GetSound(comp->mSoundAssetName);
	}
}

void AudioSystem::CacheSound(const std::string& soundName, int chuckVolumeOverride)
{
	GetSound(soundName, chuckVolumeOverride);
}



Mix_Chunk* AudioSystem::GetSound(const std::string& soundName, int chuckVolumeOverride)
{
	std::string fileName = "LocalResources/Sound/";
	fileName += soundName;

	Mix_Chunk* chunk = nullptr;
	auto itr = mSoundAssets.find(soundName);
	if (itr != mSoundAssets.end())
	{
		chunk = itr->second;
	}
	else
	{
		chunk = Mix_LoadWAV(fileName.c_str());
		if (!chunk)
		{
			printf("AudioManager::GetSound(): Cannot load sound file named %s\n", soundName.c_str());
			return nullptr;
		}
		Mix_VolumeChunk(chunk, chuckVolumeOverride);
		mSoundAssets.emplace(soundName, chunk);
	}

	return chunk;
}

void AudioSystem::Set3DEffect(int channelNum, Vec3 soundPos)
{
	Vec3 listenerToSource = soundPos - mMainListener->GetPosition();
	float dis = listenerToSource.Magnitude();
	float compressedDis = 255.0f * dis / 40.0f;
	compressedDis = Math::Clamp(compressedDis, 0.0f, 255.0f);


	listenerToSource.Normalize();
	float angle = std::atan2f(listenerToSource.mZ, listenerToSource.mX) * RAD2DEG;
	Vec3 facingDir = mMainListener->GetForward();
	facingDir.Normalize();
	angle -= std::atan2f(facingDir.mZ, facingDir.mX) * RAD2DEG;

	//TODO: Distance attenuation is pretty bad, no "cutoff". Might as well implement my own version later
	if (Mix_SetPosition(channelNum, angle, static_cast<Uint8>(compressedDis)) == 0)
	{
		printf("AudioManager::PlaySound(): Cannot set 3D effect on channel %d\n", channelNum);
	}
}

void AudioSystem::AdjustVolumeTimeline(int channelNum, int startVolume, int targetVolume, float alpha)
{
	int newVolume = Math::Lerp(startVolume, targetVolume, alpha);
	Mix_Volume(channelNum, newVolume);
}

void AudioSystem::Pause(SoundHandle sound)
{
	auto itr = mHandleMap.find(sound);

	Mix_Pause(itr->second.mChannel);
	itr->second.mIsPaused = true;
}

void AudioSystem::Stop(SoundHandle sound)
{
	auto itr = mHandleMap.find(sound);

	Mix_HaltChannel(itr->second.mChannel);
	mChannels[sound] = 0;
	mHandleMap.erase(itr);
}
