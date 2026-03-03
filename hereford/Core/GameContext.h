#pragma once

#include "Core/Actor.h"
#include <SDL2/SDL.h>
#include <vector>
#include <memory>
#include <unordered_map>
#include "Util/GameEvent.h"
#include "Input/InputCommons.h"

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

	Actor* AddActor(std::unique_ptr<Actor> actor);
	void RemoveActor(Actor* actor);

	template<typename T, typename... Args>
	T* CreateActor(Args&&... args)
	{
		auto actor = std::make_unique<T>(std::forward<Args>(args)...);
		T* raw = actor.get();
		mActors.push_back(std::move(actor));
		return raw;
	}

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

	void OnQuitInput(EInputState state);

	struct SDLWindowDeleter { void operator()(SDL_Window* w) const { SDL_DestroyWindow(w); } };
	std::unique_ptr<SDL_Window, SDLWindowDeleter> mPtrSDLWindow;
	std::unique_ptr<Renderer> mPtrRenderer;
	std::unique_ptr<PhysicsManager> mPtrPhysicsManager;
	std::unique_ptr<AudioManager> mPtrAudioManager;
	std::unique_ptr<InputManager> mPtrInputManager;


	class Player* mPtrPlayer;
	class TestMaster* mPtrTestMaster;

	bool mCursorMode;

	bool mIsRunning;
	bool mUseVerticalSync;
	Uint32 mPrevTimestamp;
	Uint32 mCurrTimestamp;
	float mDeltaTime;

	int mScreenWidth;
	int mScreenHeight;

	std::vector<std::unique_ptr<Actor>> mActors;

	hInputSub hQuitSub;

};

