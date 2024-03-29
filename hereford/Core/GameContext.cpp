#include "Core/GameContext.h"
#include "Core/Actor.h"
#include "Gameplay/Player.h"
#include "Props/TestCube.h"
#include "Util/Random.h"

#include <glad/glad.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <stdio.h>
#include <string>



GameContext::GameContext()
	: m_ScreenWidth(1920), m_ScreenHeight(1080)
{
}

GameContext::GameContext(int width, int height)
	: m_ScreenWidth(width), m_ScreenHeight(height)
{
}

GameContext::~GameContext()
{
}



bool GameContext::Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	SDL_GL_LoadLibrary(nullptr);

	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	pSDLWindow = SDL_CreateWindow("SDL Tutorial", 
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, m_ScreenWidth, m_ScreenHeight, 
		SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	if (pSDLWindow == nullptr)
	{
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	pRenderer = std::make_unique<Renderer>(pSDLWindow, m_ScreenWidth, m_ScreenHeight);

	if (!pRenderer->Initialize())
	{
		printf("Renderer could not be initialized! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	pPhysicsManager = std::make_unique<PhysicsManager>();

	//SDL_SetWindowGrab(pSDLWindow, SDL_TRUE);
	SDL_SetRelativeMouseMode(SDL_TRUE);

	isRunning = true;
	LoadData();
	return true;
}

void GameContext::Shutdown()
{
	SDL_DestroyWindow(pSDLWindow);
	pSDLWindow = nullptr;
	SDL_Quit();
}

void GameContext::RunLoop()
{
	while (isRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
	Shutdown();
}

void GameContext::LoadData()
{
	player = new Player(this);
	pRenderer->SetMainCamera(&player->GetMainCamera());

	Vector3 cubePositions[] =
	{
		Vector3(0.0f,  0.0f,  0.0f),
		Vector3(2.0f,  5.0f, -15.0f),
		Vector3(-1.5f, -2.2f, -2.5f),
		Vector3(-3.8f, -2.0f, -12.3f),
		Vector3(2.4f, -0.4f, -3.5f),
		Vector3(-1.7f,  3.0f, -7.5f),
		Vector3(1.3f, -2.0f, -2.5f),
		Vector3(1.5f,  2.0f, -2.5f),
		Vector3(1.5f,  0.2f, -1.5f),
		Vector3(-1.3f,  1.0f, -1.5f)
	};

	for (int i = 0; i < 10; i++)
	{
		TestCube* cubeActor = new TestCube(this);
		//cubeActor->SetPosition(cubePositions[i]);
		cubeActor->SetPosition(Vec3((float)i, 1.8f, 0.0f));
		//cubeActor->SetRotation(Vector3(45.0f, 0.0f, 0.0f));
	}
}

void GameContext::ProcessInput()
{
	SDL_Event test_event;
	while (SDL_PollEvent(&test_event))
	{
		switch (test_event.type)
		{
		case SDL_QUIT:
			isRunning = false;
			break;
		default:
			break;
		}
	}

	const Uint8* pKeyState = SDL_GetKeyboardState(nullptr);
	if (pKeyState[SDL_SCANCODE_ESCAPE])
		isRunning = false;

	int mouseDeltaX, mouseDeltaY;
	Uint32 pMouseState = SDL_GetRelativeMouseState(&mouseDeltaX, &mouseDeltaY);

	std::vector<Actor*> actorVector = mActors;
	for (Actor* actor : actorVector)
	{
		actor->ProcessInput(pKeyState, pMouseState, mouseDeltaX, -mouseDeltaY);
	}
}

void GameContext::UpdateGame()
{
	if (useVerticalSync)
	{
		do
		{
			currTimestamp = SDL_GetTicks();
			deltaTime = (currTimestamp - prevTimestamp) / 1000.0f;
		} while (deltaTime < 0.016);
	}
	else
	{
		currTimestamp = SDL_GetTicks();
		deltaTime = (currTimestamp - prevTimestamp) / 1000.0f;
	}
	prevTimestamp = currTimestamp;

	std::vector<Actor*> actorVector = mActors;
	std::vector<Actor*> disabledActorVector;

	for (Actor* actor : actorVector)
	{
		actor->Update(deltaTime);
		if (actor->GetState() == ActorState::Destroy)
			disabledActorVector.push_back(actor);
	}

	for (Actor* actor : disabledActorVector)
	{
		delete actor;
	}
}

void GameContext::GenerateOutput()
{
	pRenderer->Render(deltaTime);
}

void GameContext::AddActor(Actor* actor)
{
	mActors.push_back(actor);
}

void GameContext::RemoveActor(Actor* actor)
{
	mActors.erase(std::find(mActors.begin(), mActors.end(), actor));

}
