#include "AudioManager.h"
#include "Core/Actor.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"
#include "imgui/imgui.h"


AudioManager::AudioManager(int channelNum)
	: mPlayerRef(nullptr)
{
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	Mix_AllocateChannels(channelNum);
	mChannels.resize(channelNum);
}

AudioManager::~AudioManager()
{
	for (auto& s : mSoundAssets)
	{
		Mix_FreeChunk(s.second);
	}
	mSoundAssets.clear();
	Mix_CloseAudio();
}

void AudioManager::Update()
{
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
			if(mHandleMap[mChannels[i]].mIs3D)
				Set3DEffect(i, mHandleMap[mChannels[i]].mSoundPos);
		}
	}
}

// TODO Features:
// Sound fade out when pausing/stopping
// Sound playback speed

SoundHandle AudioManager::PlaySound(const std::string& soundName, bool shouldLooping, bool is3D, Vec3 soundPos)
{
	Mix_Chunk* pChunk = GetSound(soundName);
	if (pChunk == nullptr)
	{
		printf("AudioManager::PlaySound(): Unable to get sound asset named [%s]\n", soundName.c_str());
		return 0;
	}

	for (int i = 0; i < mChannels.size(); i++)
	{
		if (mChannels[i] == 0)
		{
			mLastHandle++;
			HandleInfo handle(soundName, i, shouldLooping, false, is3D, soundPos);
			mHandleMap[mLastHandle] = handle;
			mChannels[i] = mLastHandle;
			Mix_Volume(-1, 128);
			Mix_PlayChannel(i, pChunk, shouldLooping ? -1 : 0);

			break;
		}
	}

	//TODO: Prioritization

	/*The oldest instance of the same soundName and if there is none…
		The oldest non - looping sound and if there is none…
		The oldest sound*/


	return mLastHandle;
}

void AudioManager::StopSound(SoundHandle sound)
{
	auto itr = mHandleMap.find(sound);
	if (itr == mHandleMap.end())
	{
		printf("AudioManager::StopSound(): Unable to find sound handle of id [%d]\n", sound);
		return;
	}

	Mix_HaltChannel(itr->second.mChannel);
	mChannels[sound] = 0;
	mHandleMap.erase(itr);
}

void AudioManager::PauseSound(SoundHandle sound)
{
	auto itr = mHandleMap.find(sound);
	if (itr == mHandleMap.end())
	{
		printf("AudioManager::PauseSound(): Unable to find sound handle of id [%d]\n", sound);
		return;
	}

	if (!itr->second.mIsPaused)
	{
		Mix_Pause(itr->second.mChannel);
		itr->second.mIsPaused = true;
	}
}

void AudioManager::ResumeSound(SoundHandle sound)
{
	auto itr = mHandleMap.find(sound);
	if (itr == mHandleMap.end())
	{
		printf("AudioManager::ResumeSound(): Unable to find sound handle of id [%d]\n", sound);
		return;
	}

	if (itr->second.mIsPaused)
	{
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
	auto itr = mSoundAssets.find(fileName);
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

		mSoundAssets.emplace(fileName, chunk);
	}
	
	return chunk;
}

void AudioManager::Set3DEffect(int channelNum, Vec3 soundPos)
{
	Vec3 listenerToSource = soundPos - mPlayerRef->GetPosition();
	float dis = listenerToSource.Magnitude();
	float compressedDis = 255.0f * dis / 40.0f;
	compressedDis = Math::Clamp(compressedDis, 0.0f, 255.0f);


	listenerToSource.Normalize();
	float angle = std::atan2f(listenerToSource.mZ, listenerToSource.mX) * RAD2DEG;
	Vec3 facingDir = mPlayerRef->GetForward();
	facingDir.Normalize();
	angle -= std::atan2f(facingDir.mZ, facingDir.mX) * RAD2DEG;

	//TODO: Distance attenuation is pretty bad, no "cutoff". Might as well implement my own version later
	if (Mix_SetPosition(channelNum, angle, static_cast<Uint8>(compressedDis)) == 0)
	{
		printf("AudioManager::PlaySound(): Cannot set 3D effect on channel %d\n", channelNum);
	}

	ImGui::Begin("Sound Test");
	ImGui::Text("Angle: %f\n", angle);
	ImGui::Text("Dis: %f\n", dis);
	ImGui::End();
}
