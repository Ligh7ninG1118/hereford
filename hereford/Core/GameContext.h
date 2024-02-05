#pragma once

#include <SDL2/SDL.h>
#include <vector>


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

private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	SDL_Window* pSDLWindow;
	class Renderer* pRenderer;

	bool isRunning;
	Uint32 prevTimestamp;
	Uint32 currTimestamp;
	float deltaTime;

	int m_ScreenWidth;
	int m_ScreenHeight;

	std::vector<class Actor*> mActors;



};

