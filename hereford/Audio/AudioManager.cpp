#include "AudioManager.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"


AudioManager::AudioManager(int channelNum)
{
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	Mix_AllocateChannels(channelNum);
	mChannels.resize(channelNum);
}

AudioManager::~AudioManager()
{
	for (auto& s : mSounds)
	{
		Mix_FreeChunk(s.second);
	}
	mSounds.clear();
	Mix_CloseAudio();
}

void AudioManager::Update()
{
	for (auto& c : mChannels)
	{
		if (c != 0 && !Mix_Playing(c))
		{
			mHandleMap.erase(c);
			c = 0;
		}
	}
}

SoundHandle AudioManager::PlaySound(const std::string& soundName, bool shouldLooping)
{
	Mix_Chunk* pChunk = GetSound(soundName);
	if (pChunk == nullptr)
	{
		printf("");
		return 0;
	}

	for (int i = 0; i < mChannels.size(); i++)
	{
		if (mChannels[i] == 0)
		{
			mLastHandle++;
			HandleInfo handle(soundName, i, shouldLooping, false);
			mHandleMap[mLastHandle] = handle;
			Mix_Volume(-1, 8);
			//Mix_VolumeChunk
			//Mix_SetPosition();
			Mix_PlayChannel(i, pChunk, shouldLooping ? -1 : 0);
		}
	}

	return mLastHandle;
}

void AudioManager::StopSound(SoundHandle sound)
{
	auto itr = mHandleMap.find(sound);
	if (itr == mHandleMap.end())
	{
		printf("");
		return;
	}
	//pass in sound??
	Mix_HaltChannel(itr->second.mChannel);
	mChannels[sound] = 0;
	mHandleMap.erase(itr);
}

void AudioManager::PauseSound(SoundHandle sound)
{
	auto itr = mHandleMap.find(sound);
	if (itr == mHandleMap.end())
	{
		printf("");
		return;
	}

	if (!itr->second.mIsPaused)
	{
		//pass in sound??
		Mix_Pause(itr->second.mChannel);
		itr->second.mIsPaused = true;
	}
}

void AudioManager::ResumeSound(SoundHandle sound)
{
	auto itr = mHandleMap.find(sound);
	if (itr == mHandleMap.end())
	{
		printf("");
		return;
	}

	if (itr->second.mIsPaused)
	{
		//pass in sound??
		Mix_Resume(itr->second.mChannel);
		itr->second.mIsPaused = true;
	}
}

ESoundState AudioManager::GetSoundState(SoundHandle sound)
{
	auto itr = mHandleMap.find(sound);
	if (itr == mHandleMap.end())
		return ESoundState::Stopped;

	return itr->second.mIsPaused ? ESoundState::Paused : ESoundState::Playing;
}

void AudioManager::StopAllSounds()
{
	Mix_HaltChannel(-1);
	for (auto& c : mChannels)
	{
		c = 0;
	}
	mHandleMap.clear();
}

void AudioManager::CacheAllSounds()
{
	/*for (const auto& rootDirEntry : std::filesystem::directory_iterator{ "Assets/Sounds", ec })
	{
		std::string extension = rootDirEntry.path().extension().string();
		if (extension == ".ogg" || extension == ".wav")
		{
			std::string fileName = rootDirEntry.path().stem().string();
			fileName += extension;
			CacheSound(fileName);
		}
	}*/
}

void AudioManager::CacheSound(const std::string& soundName)
{
	GetSound(soundName);
}

Mix_Chunk* AudioManager::GetSound(const std::string& soundName)
{
	std::string fileName = "LocalResources/Sound/";
	fileName += soundName;

	Mix_Chunk* chunk = nullptr;
	auto itr = mSounds.find(fileName);
	if (itr != mSounds.end())
	{
		chunk = itr->second;
	}
	else
	{
		chunk = Mix_LoadWAV(fileName.c_str());
		if (!chunk)
		{
			printf("");
			return nullptr;
		}

		mSounds.emplace(fileName, chunk);
	}
	
	return chunk;
}
