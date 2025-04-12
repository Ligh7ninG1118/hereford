#pragma once

#include "Core/Actor.h"
#include <SDL2/SDL.h>
#include <vector>
#include <memory>
#include <unordered_map>
#include "Util/GameEvent.h"

class Renderer;
class AudioManager;
class PhysicsManager;
class InputManager;


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
	InputManager& GetInputManager() const { return *mPtrInputManager.get(); }

	class Player* GetPlayer() const { return mPtrPlayer; }

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
	std::unique_ptr<InputManager> mPtrInputManager;


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

	int mScreenWidth;
	int mScreenHeight;

	std::vector<Actor*> mActors;

};

