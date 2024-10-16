#include "Core/GameContext.h"
#include "Core/Actor.h"
#include "Gameplay/Player.h"
#include "Props/TestCube.h"
#include "Props/Ground.h"
#include "Props/LightBulb.h"
#include "Props/NPC.h"
#include "Props/PlywoodWall.h"
#include "Util/Random.h"
#include "Util/DelayedAction.h"
#include "Util/TimelineAction.h"
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
	mPrevKeyStates.resize(232);
	mPrevMouseStates = EMouseState::LMB_NOT_PRESSED | EMouseState::MMB_NOT_PRESSED | EMouseState::RMB_NOT_PRESSED | EMouseState::SCROLL_IDLE;

	mTelemetryUpdateInterval = 1.0f;
	mTelemetryUpdateTimer = 0.0f;

	mCursorMode = false;
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
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
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

	mPtrAudioManager = std::make_unique<AudioManager>();
	mPtrAudioManager->CacheSound("bootcamp_ambient.wav");


	//SDL_SetWindowGrab(pSDLWindow, SDL_TRUE);
	SDL_SetRelativeMouseMode(SDL_TRUE);

	mIsRunning = true;
	LoadStarterData();
	LoadScene("Scenes/playground-target.json");

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	ImGui::StyleColorsDark();
	ImGui_ImplSDL2_InitForOpenGL(mPtrSDLWindow, mPtrRenderer->GetGLContext());
	ImGui_ImplOpenGL3_Init();

	mPtrAudioManager->PlaySound("bootcamp_ambient.wav", true, false);

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
		UpdateAudio();
		CalculatePhysics();
		DebugSceneObjects();
		DelayedActionManager::UpdateTimers(mDeltaTime);
		TimelineActionManager::UpdateTimers(mDeltaTime);
		Uint32 timestampUpdate = SDL_GetTicks();
		GenerateOutput();
		Uint32 timestampRender = SDL_GetTicks();

		mTelemetryUpdateTimer += mDeltaTime;
		if (mTelemetryUpdateTimer >= mTelemetryUpdateInterval)
		{
			mTelemetryUpdateTimer = 0.0f;
			cpuTime = timestampUpdate - timestampStart;
			gpuTime = timestampRender - timestampUpdate;
		}
	}
	Shutdown();
}

void GameContext::LoadStarterData()
{
	mPtrPlayer = new Player(this);
	mPtrRenderer->SetMainCamera(&mPtrPlayer->GetMainCamera());

	mPtrAudioManager->SetPlayerReference(mPtrPlayer);
}

void GameContext::LoadScene(const std::string& sceneFilePath)
{
	//ClearScene();

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
		Actor* pActor = nullptr;
		if (actor["className"] == "PlywoodWall")
		{
			pActor = new PlywoodWall(this, actor["type"]);
		}
		else
		{
			pActor = ReflectionRegistry::Instance().CreateInstance(actor["className"], this);

		}
		if (pActor == nullptr)
		{
			printf("GameContext::LoadScene(): Class[%s] not found in registry\n", static_cast<std::string>(actor["className"]).c_str());
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
		if (!actor["scale"].empty())
		{
			Vec3 scale = Vec3(static_cast<float>(actor["scale"][0]), static_cast<float>(actor["scale"][1]), static_cast<float>(actor["scale"][2]));
			pActor->SetScale(scale);
		}

	}

}

void GameContext::ClearScene()
{
	while (!mActors.empty())
	{
		if (dynamic_cast<Player*>(mActors.back()))
			break;

		delete mActors.back();
	}
}

void GameContext::SaveScene(const std::string& sceneFilePath)
{
	using json = nlohmann::json;
	json scene;

	for (const auto& actor : mActors)
	{
		if (dynamic_cast<Player*>(actor))
			continue;

		json actorJson;
		actorJson["className"] = actor->GetClassName();

		Vec3 pos = actor->GetPosition();
		actorJson["position"] = { pos.mX, pos.mY, pos.mZ };

		Vec3 rot = actor->GetRotation();
		actorJson["rotation"] = { rot.mX, rot.mY, rot.mZ };

		Vec3 scale = actor->GetScale();
		actorJson["scale"] = { scale.mX, scale.mY, scale.mZ };

		scene["actors"].push_back(actorJson);
	}

	std::ofstream file(sceneFilePath.c_str());
	if (!file.is_open())
	{
		printf("GameContext::SaveScene(): Could not open scene file for writing\n");
		return;
	}

	file << scene.dump(4); // Pretty print with 4 spaces
	file.close();
}


void GameContext::ProcessInput()
{
	SDL_Event pollEvent;
	int mouseWheel = 0;
	while (SDL_PollEvent(&pollEvent))
	{
		ImGui_ImplSDL2_ProcessEvent(&pollEvent);
		switch (pollEvent.type)
		{
		case SDL_MOUSEWHEEL:
			mouseWheel = pollEvent.wheel.y;
			break;
		case SDL_QUIT:
			mIsRunning = false;
			break;
		default:
			break;
		}
	}

	const Uint8* rawKeyState = SDL_GetKeyboardState(nullptr);
	int mouseDeltaX = 0, mouseDeltaY = 0;
	Uint32 newMouseState = 0;

	if (rawKeyState[SDL_SCANCODE_ESCAPE])
		mIsRunning = false;

	for (int i = 0; i < mPrevKeyStates.size(); i++)
	{
		if (mPrevKeyStates[i] == EInputState::KEY_HOLD || mPrevKeyStates[i] == EInputState::KEY_DOWN)
			mPrevKeyStates[i] = rawKeyState[i] ? EInputState::KEY_HOLD : EInputState::KEY_UP;
		else
			mPrevKeyStates[i] = rawKeyState[i] ? EInputState::KEY_DOWN : EInputState::NOT_PRESSED;
	}

	if (mPrevKeyStates[SDL_SCANCODE_GRAVE] == EInputState::KEY_DOWN)
	{
		mCursorMode = !mCursorMode;
		SDL_SetRelativeMouseMode(static_cast<SDL_bool>(!mCursorMode));
		// Clear mouse state to avoid sudden view change
		SDL_GetRelativeMouseState(nullptr, nullptr);
	}

	if (!mCursorMode)
	{
		Uint32 rawMouseState = SDL_GetRelativeMouseState(&mouseDeltaX, &mouseDeltaY);

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

		switch (mouseWheel)
		{
		case 0:
			newMouseState |= EMouseState::SCROLL_IDLE;
			break;
		case 1:
			newMouseState |= EMouseState::SCROLL_UP;
			break;
		case -1:
			newMouseState |= EMouseState::SCROLL_DOWN;
			break;
		}

		mPrevMouseStates = newMouseState;
	}
	else
	{
		EInputState cacheState = mPrevKeyStates[SDL_SCANCODE_GRAVE];
		mPrevKeyStates.assign(mPrevKeyStates.size(), EInputState::NOT_PRESSED);
		mPrevKeyStates[SDL_SCANCODE_GRAVE] = cacheState;
	}

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
		if (actor->GetState() == EActorState::Destroy)
			disabledActorVector.push_back(actor);
	}

	for (Actor* actor : disabledActorVector)
	{
		delete actor;
	}
}

void GameContext::UpdateAudio()
{
	mPtrAudioManager->Update();
}

void GameContext::CalculatePhysics()
{
	mPtrPhysicsManager->UpdatePhysics(mDeltaTime);

}

void GameContext::GenerateOutput()
{
	mPtrRenderer->Render(mDeltaTime);
}

void GameContext::DebugSceneObjects()
{
	//ImGui::ShowDemoWindow();
	ImGui::Begin("Scene Editing", 0, ImGuiWindowFlags_AlwaysAutoResize);
	if (ImGui::TreeNode("Save Scene"))
	{
		static char saveSceneName[64] = "";
		ImGui::InputText("Save Scene Name", saveSceneName, IM_ARRAYSIZE(saveSceneName));
		if (ImGui::Button("Save Scene"))
			SaveScene("Scenes/" + std::string(saveSceneName) + ".json");
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Load Scene"))
	{
		static char loadSceneName[64] = "";
		ImGui::InputText("Load Scene Name", loadSceneName, IM_ARRAYSIZE(loadSceneName));
		if (ImGui::Button("Load Scene"))
			LoadScene("Scenes/" + std::string(loadSceneName) + ".json");
		ImGui::TreePop();
	}


	if (ImGui::TreeNode("Scene Objects"))
	{
		int id = 0;
		for (const auto& actor : mActors)
		{
			if (ImGui::TreeNode((actor->GetClassName() + std::to_string(id)).c_str()))
			{
				Vec3 pos = actor->GetPosition();
				ImGui::Text("Position");
				ImGui::SliderFloat("X", &pos.mX, -100.0f, 100.0f);
				ImGui::SameLine();
				ImGui::SliderFloat("Y", &pos.mY, -100.0f, 100.0f);
				ImGui::SameLine();
				ImGui::SliderFloat("Z", &pos.mZ, -100.0f, 100.0f);
				actor->SetPosition(pos);

				Vec3 rot = actor->GetRotation();
				ImGui::Text("Rotation");
				ImGui::SliderFloat("Roll", &rot.mX, -360.0f, 360.0f);
				ImGui::SameLine();
				ImGui::SliderFloat("Pitch", &rot.mY, -360.0f, 360.0f);
				ImGui::SameLine();
				ImGui::SliderFloat("Yaw", &rot.mZ, -360.0f, 360.0f);
				actor->SetRotation(rot);

				Vec3 scale = actor->GetScale();
				ImGui::Text("Scale");
				ImGui::SliderFloat("Scale X", &scale.mX, -100.0f, 100.0f);
				ImGui::SameLine();
				ImGui::SliderFloat("Scale Y", &scale.mY, -100.0f, 100.0f);
				ImGui::SameLine();
				ImGui::SliderFloat("Scale Z", &scale.mZ, -100.0f, 100.0f);
				actor->SetScale(scale);

				ImGui::TreePop();
			}
			id++;
		}
		ImGui::TreePop();
	}

	
	if (ImGui::TreeNode("Add Object"))
	{
		ImGui::Text("Add Actor");
		static char actorClass[64] = "";
		ImGui::InputText("Class Name", actorClass, IM_ARRAYSIZE(actorClass));

		if (ImGui::Button("Add"))
		{
			ReflectionRegistry::Instance().CreateInstance(std::string(actorClass), this);
		}
		ImGui::TreePop();
	}
	
	ImGui::End();
}



void GameContext::AddActor(Actor* actor)
{
	mActors.push_back(actor);
}

void GameContext::RemoveActor(Actor* actor)
{
	mActors.erase(std::find(mActors.begin(), mActors.end(), actor));

}
