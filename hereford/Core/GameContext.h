#pragma once

#include "Graphics/Renderer.h"
#include "Core/Actor.h"
#include "Physics/PhysicsManager.h"
#include <SDL2/SDL.h>
#include <vector>
#include <memory>
#include <unordered_map>


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

	Uint32 GetShader(std::string vertexPath, std::string fragPath);
	Uint32 GetMesh(std::string meshPath);


private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	SDL_Window* pSDLWindow;
	std::unique_ptr<Renderer> pRenderer;
	std::unique_ptr<PhysicsManager> pPhysicsManager;
	class Player* player;

	std::unordered_map<std::string, Uint32> m_ShaderMap;
	std::unordered_map<std::string, Uint32> m_MeshMap;

	bool isRunning;
	bool useVerticalSync;
	Uint32 prevTimestamp;
	Uint32 currTimestamp;
	float deltaTime;

	int m_ScreenWidth;
	int m_ScreenHeight;

	std::vector<Actor*> mActors;



};

