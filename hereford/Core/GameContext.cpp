#include "Core/GameContext.h"
#include "Core/Actor.h"
#include "Gameplay/Player.h"
#include "Props/TestCube.h"
#include "Props/Ground.h"
#include "Props/LightBulb.h"
#include "Props/NPC.h"
#include "Util/Random.h"
#include "Util/DelayedAction.h"
#include "Asset/Shader.h"

#include "Asset/Model.h"

#include <glad/glad.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <stdio.h>
#include <string>

#include <nlohmann/json.hpp>
#include <fstream>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl2.h"
#include "imgui/imgui_impl_opengl3.h"



GameContext::GameContext()
	: mScreenWidth(1920), mScreenHeight(1080)
{
	// Enough for most input key usage
	mPrevKeyStates.resize(70);
	mPrevMouseStates = EMouseState::LMB_NOT_PRESSED | EMouseState::MMB_NOT_PRESSED | EMouseState::RMB_NOT_PRESSED;
}

GameContext::GameContext(int width, int height)
	: mScreenWidth(width), mScreenHeight(height)
{
}

GameContext::~GameContext()
{
	//printf("GameContext Destructor\n");
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

	mPtrSDLWindow = SDL_CreateWindow("Hereford",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, mScreenWidth, mScreenHeight,
		SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	if (mPtrSDLWindow == nullptr)
	{
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	mPtrRenderer = std::make_unique<Renderer>(mPtrSDLWindow, this, mScreenWidth, mScreenHeight);

	if (!mPtrRenderer->Initialize())
	{
		printf("Renderer could not be initialized! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	mPtrPhysicsManager = std::make_unique<PhysicsManager>();

	//SDL_SetWindowGrab(pSDLWindow, SDL_TRUE);
	SDL_SetRelativeMouseMode(SDL_TRUE);

	mIsRunning = true;
	LoadData();
	LoadScene("Scenes/playground.json");

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	ImGui::StyleColorsDark();
	ImGui_ImplSDL2_InitForOpenGL(mPtrSDLWindow, mPtrRenderer->GetGLContext());
	ImGui_ImplOpenGL3_Init();

	return true;
}

void GameContext::Shutdown()
{
	while (!mActors.empty())
	{
		delete mActors.back();
	}

	mPtrRenderer->Shutdown();
	
	ImGui::DestroyContext();

	SDL_DestroyWindow(mPtrSDLWindow);
	mPtrSDLWindow = nullptr;
	SDL_Quit();
}

void GameContext::RunLoop()
{
	while (mIsRunning)
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();


		ImGui::Begin("Telemetry");
		ImGui::Text("CPU Frame Time %d ms \t GPU Frame Time %d ms \nFPS: %.2f", cpuTime, gpuTime, 1000.0f / (cpuTime+gpuTime));
		ImGui::End();

		Uint32 timestampStart = SDL_GetTicks();
		ProcessInput();
		UpdateGame();
		CalculatePhysics();
		DelayedActionManager::UpdateTimers(mDeltaTime);
		Uint32 timestampUpdate = SDL_GetTicks();
		cpuTime = timestampUpdate - timestampStart;

		GenerateOutput();
		Uint32 timestampRender = SDL_GetTicks();
		gpuTime = timestampRender - timestampUpdate;
		
	}
	Shutdown();
}

void GameContext::LoadData()
{
	mPtrPlayer = new Player(this);
	mPtrRenderer->SetMainCamera(&mPtrPlayer->GetMainCamera());

	Ground* ground = new Ground(this);
}

void GameContext::LoadScene(const std::string& sceneFilePath)
{
	using json = nlohmann::json;

	std::fstream file(sceneFilePath.c_str());
	if (!file.is_open())
	{
		printf("GameContext::LoadScene(): Scene file not found\n");
		return;
	}

	json scene = json::parse(file);
	file.close();

	unsigned int actorCount = (unsigned int)scene["actors"].size();
	for (unsigned int i = 0; i < actorCount; i++)
	{
		json actor = scene["actors"][i];
		Actor* pActor = ReflectionRegistry::Instance().CreateInstance(actor["class"], this);
		if (pActor == nullptr)
		{
			printf("GameContext::LoadScene(): Class[%s] not found in registry\n", static_cast<std::string>(actor["class"]).c_str());
			continue;
		}

		if (!actor["position"].empty())
		{
			Vec3 pos = Vec3(static_cast<float>(actor["position"][0]), static_cast<float>(actor["position"][1]), static_cast<float>(actor["position"][2]));
			pActor->SetPosition(pos);
		}
		if (!actor["rotation"].empty())
		{
			Vec3 rot = Vec3(static_cast<float>(actor["rotation"][0]), static_cast<float>(actor["rotation"][1]), static_cast<float>(actor["rotation"][2]));
			pActor->SetRotation(rot);
		}

	}

}


void GameContext::ProcessInput()
{
	SDL_Event pollEvent;
	while (SDL_PollEvent(&pollEvent))
	{
		ImGui_ImplSDL2_ProcessEvent(&pollEvent);
		switch (pollEvent.type)
		{
		case SDL_QUIT:
			mIsRunning = false;
			break;
		default:
			break;
		}
	}

	const Uint8* rawKeyState = SDL_GetKeyboardState(nullptr);
	if (rawKeyState[SDL_SCANCODE_ESCAPE])
		mIsRunning = false;

	for (int i = 0; i < mPrevKeyStates.size(); i++)
	{
		if (mPrevKeyStates[i] == EInputState::KEY_HOLD || mPrevKeyStates[i] == EInputState::KEY_DOWN)
			mPrevKeyStates[i] = rawKeyState[i] ? EInputState::KEY_HOLD : EInputState::KEY_UP;
		else
			mPrevKeyStates[i] = rawKeyState[i] ? EInputState::KEY_DOWN : EInputState::NOT_PRESSED;
	}

	int mouseDeltaX, mouseDeltaY;
	Uint32 rawMouseState = SDL_GetRelativeMouseState(&mouseDeltaX, &mouseDeltaY);
	Uint32 newMouseState = 0;

	if ((mPrevMouseStates & EMouseState::LMB_HOLD) || (mPrevMouseStates & EMouseState::LMB_DOWN))
		newMouseState |= (rawMouseState & SDL_BUTTON_LMASK) ? EMouseState::LMB_HOLD : EMouseState::LMB_UP;
	else if ((mPrevMouseStates & EMouseState::LMB_NOT_PRESSED) || (mPrevMouseStates & EMouseState::LMB_UP))
		newMouseState |= (rawMouseState & SDL_BUTTON_LMASK) ? EMouseState::LMB_DOWN : EMouseState::LMB_NOT_PRESSED;

	if ((mPrevMouseStates & EMouseState::RMB_HOLD) || (mPrevMouseStates & EMouseState::RMB_DOWN))
		newMouseState |= (rawMouseState & SDL_BUTTON_RMASK) ? EMouseState::RMB_HOLD : EMouseState::RMB_UP;
	else if ((mPrevMouseStates & EMouseState::RMB_NOT_PRESSED) || (mPrevMouseStates & EMouseState::RMB_UP))
		newMouseState |= (rawMouseState & SDL_BUTTON_RMASK) ? EMouseState::RMB_DOWN : EMouseState::RMB_NOT_PRESSED;

	if ((mPrevMouseStates & EMouseState::MMB_HOLD) || (mPrevMouseStates & EMouseState::MMB_DOWN))
		newMouseState |= (rawMouseState & SDL_BUTTON_MMASK) ? EMouseState::MMB_HOLD : EMouseState::MMB_UP;
	else if ((mPrevMouseStates & EMouseState::MMB_NOT_PRESSED) || (mPrevMouseStates & EMouseState::MMB_UP))
		newMouseState |= (rawMouseState & SDL_BUTTON_MMASK) ? EMouseState::MMB_DOWN : EMouseState::MMB_NOT_PRESSED;

	mPrevMouseStates = newMouseState;

	std::vector<Actor*> actorVector = mActors;
	for (Actor* actor : actorVector)
	{
		actor->ProcessInput(mPrevKeyStates, mPrevMouseStates, mouseDeltaX, -mouseDeltaY);
	}

}

void GameContext::UpdateGame()
{
	currTimestamp = SDL_GetTicks();
	mDeltaTime = (currTimestamp - prevTimestamp) / 1000.0f;
	prevTimestamp = currTimestamp;

	std::vector<Actor*> actorVector = mActors;
	std::vector<Actor*> disabledActorVector;

	for (Actor* actor : actorVector)
	{
		actor->Update(mDeltaTime);
		if (actor->GetState() == ActorState::Destroy)
			disabledActorVector.push_back(actor);
	}

	for (Actor* actor : disabledActorVector)
	{
		delete actor;
	}
}

void GameContext::CalculatePhysics()
{
	mPtrPhysicsManager->UpdatePhysics(mDeltaTime);

}

void GameContext::GenerateOutput()
{
	mPtrRenderer->Render(mDeltaTime);
}



void GameContext::AddActor(Actor* actor)
{
	mActors.push_back(actor);
}

void GameContext::RemoveActor(Actor* actor)
{
	mActors.erase(std::find(mActors.begin(), mActors.end(), actor));

}
