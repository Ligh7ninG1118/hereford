#pragma once

#include "Core/Actor.h"

#include "Audio/AudioManager.h"
#include "Graphics/Renderer.h"
#include "Physics/PhysicsManager.h"

#include <SDL2/SDL.h>
#include <vector>
#include <memory>
#include <unordered_map>

#include "Util/GameEvent.h"

class GameContext
{
public:
	GameContext();
	GameContext(int width, int height);
	~GameContext();

	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);

	bool Initialize();
	void Shutdown();
	void RunLoop();

	Renderer& GetRenderer() const { return *mPtrRenderer.get(); }
	PhysicsManager& GetPhysicsManager() const { return *mPtrPhysicsManager.get(); }
	AudioManager& GetAudioManager() const { return *mPtrAudioManager.get(); }

	class Player* GetPlayer() const { return mPtrPlayer; }

	//TODO: Move to Telemetry Class
	uint32 cpuTime;
	uint32 gpuTime;

private:
	void LoadStarterData();
	void LoadCubeTest();
	void SaveScene(const std::string& sceneFilePath);
	void LoadScene(const std::string& sceneFilePath);
	void ClearScene();

	void ProcessInput();
	void UpdateGame();
	void UpdateAudio();
	void CalculatePhysics();
	void GenerateOutput();

	void DebugSceneObjects();

	SDL_Window* mPtrSDLWindow;
	std::unique_ptr<Renderer> mPtrRenderer;
	std::unique_ptr<PhysicsManager> mPtrPhysicsManager;
	std::unique_ptr<AudioManager> mPtrAudioManager;

	class Player* mPtrPlayer;
	class TestMaster* mPtrTestMaster;


	// separate asset manager to manage this
	std::unordered_map<std::string, Uint32> mShaderMap;
	std::unordered_map<std::string, Uint32> mMeshMap;


	std::vector<EInputState> mPrevKeyStates;

	Uint32 mPrevMouseStates;

	bool mCursorMode;

	bool mIsRunning;
	bool useVerticalSync;
	Uint32 prevTimestamp;
	Uint32 currTimestamp;
	float mDeltaTime;

	float mTelemetryUpdateInterval;
	float mTelemetryUpdateTimer;

	int mScreenWidth;
	int mScreenHeight;

	std::vector<Actor*> mActors;

};

