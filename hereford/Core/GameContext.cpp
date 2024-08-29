#include "Core/GameContext.h"
#include "Core/Actor.h"
#include "Gameplay/Player.h"
#include "Props/TestCube.h"
#include "Props/LightBulb.h"
#include "Util/Random.h"
#include "Asset/Shader.h"

#include "Asset/Model.h"

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

	pRenderer = std::make_unique<Renderer>(pSDLWindow, this, m_ScreenWidth, m_ScreenHeight);

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
		Uint32 timestampStart = SDL_GetTicks();
		ProcessInput();
		Uint32 timestampAfterInput = SDL_GetTicks();
		//printf("Process Input: %d ms \n", timestampAfterInput - timestampStart);
		UpdateGame();
		Uint32 timestampUpdate = SDL_GetTicks();
		//printf("Update Game: %d ms \n", timestampUpdate - timestampAfterInput);
		GenerateOutput();
		Uint32 timestampRender = SDL_GetTicks();
		//printf("Render: %d ms \n", timestampRender - timestampUpdate);
	}
	Shutdown();
}

void GameContext::LoadData()
{
	player = new Player(this);
	pRenderer->SetMainCamera(&player->GetMainCamera());

	/*for (int i = 0; i < 10; i++)
	{
		TestCube* cubeActor = new TestCube(this);
		cubeActor->SetPosition(Vec3(Random::Range(-20.0f, 20.0f), Random::Range(-20.0f, 20.0f), Random::Range(-20.0f, 20.0f) ));
	}


	LightBulb* lightBulb = new LightBulb(this);
	lightBulb->SetPosition(Vec3(0.0f, 5.0f, 0.0f));

	LightBulb* lightBulb2 = new LightBulb(this);
	lightBulb2->SetPosition(Vec3(0.0f, -2.0f, 0.0f));*/


	testEventSub = GameEvent::Subscribe(std::bind(&GameContext::EventListner, this, std::placeholders::_1));

}


void GameContext::EventListner(EventTest inEvent)
{
	printf("Event received. Value: %d\n", inEvent.mX);
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
	currTimestamp = SDL_GetTicks();
	deltaTime = (currTimestamp - prevTimestamp) / 1000.0f;
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
