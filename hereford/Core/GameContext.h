#pragma once

#include "../Graphics/Renderer.h"
#include "Actor.h"
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

	Renderer* GetRenderer() const { return pRenderer; }

private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	SDL_Window* pSDLWindow;
	Renderer* pRenderer;
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

