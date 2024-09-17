#pragma once

#include "Graphics/Renderer.h"
#include "Core/Actor.h"
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

	class Player* GetPlayer() const { return mPtrPlayer; }

	//TODO: Move to Telemetry Class
	uint32 cpuTime;
	uint32 gpuTime;

private:
	

	void LoadData();
	void LoadScene(const std::string& sceneFilePath);

	void ProcessInput();
	void UpdateGame();
	void CalculatePhysics();
	void GenerateOutput();

	SDL_Window* mPtrSDLWindow;
	std::unique_ptr<Renderer> mPtrRenderer;
	std::unique_ptr<PhysicsManager> mPtrPhysicsManager;
	class Player* mPtrPlayer;

	// separate asset manager to manage this
	std::unordered_map<std::string, Uint32> mShaderMap;
	std::unordered_map<std::string, Uint32> mMeshMap;


	std::vector<EInputState> mPrevKeyStates;

	Uint32 mPrevMouseStates;

	bool mIsRunning;
	bool useVerticalSync;
	Uint32 prevTimestamp;
	Uint32 currTimestamp;
	float mDeltaTime;

	int mScreenWidth;
	int mScreenHeight;

	std::vector<Actor*> mActors;

};

