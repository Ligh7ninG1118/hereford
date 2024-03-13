#pragma once

#include "Graphics/Renderer.h"
#include "Core/Actor.h"
#include "Physics/PhysicsManager.h"
#include <SDL2/SDL.h>
#include <vector>
#include <memory>


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

	void LoadData();

	Renderer& GetRenderer() const { return *pRenderer.get(); }
	PhysicsManager& GetPhysicsManager() const { return *pPhysicsManager.get(); }


private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	SDL_Window* pSDLWindow;
	std::unique_ptr<Renderer> pRenderer;
	std::unique_ptr<PhysicsManager> pPhysicsManager;
	class Player* player;

	bool isRunning;
	bool useVerticalSync;
	Uint32 prevTimestamp;
	Uint32 currTimestamp;
	float deltaTime;

	int m_ScreenWidth;
	int m_ScreenHeight;

	std::vector<Actor*> mActors;



};

