#include "Core/GameContext.h"
#include "Core/Actor.h"
#include "Graphics/Renderer.h"
#include "Audio/AudioManager.h"
#include "Physics/PhysicsManager.h"
#include "Gameplay/Player.h"
#include "Gameplay/FlyCamera.h"
#include "Gameplay/TestMaster.h"
#include "Props/PlywoodWall.h"
#include "Props/TestCube.h"
#include "Input/InputManager.h"
#include "Util/DelayedAction.h"
#include "Util/TimelineAction.h"
#include "Util/Profiler.h"

#include <glad/glad.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <cstdio>
#include <string>

#include <nlohmann/json.hpp>
#include <fstream>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl2.h"
#include "imgui/imgui_impl_opengl3.h"

GameContext::GameContext()
	: mScreenWidth(1920), mScreenHeight(1080),
	mPtrPlayer(nullptr),
	mPtrTestMaster(nullptr),
	mCursorMode(false),
	mIsRunning(false),
	mUseVerticalSync(false),
	mPrevTimestamp(0),
	mCurrTimestamp(0),
	mDeltaTime(0.0f),
	hQuitSub(0)
{
}

GameContext::GameContext(int width, int height)
	: mScreenWidth(width), mScreenHeight(height),
	mPtrPlayer(nullptr),
	mPtrTestMaster(nullptr),
	mCursorMode(false),
	mIsRunning(false),
	mUseVerticalSync(false),
	mPrevTimestamp(0),
	mCurrTimestamp(0),
	mDeltaTime(0.0f),
	hQuitSub(0)
{
}

GameContext::~GameContext()
{
}



bool GameContext::Initialize()
{
	SDL_SetHint(SDL_HINT_WINDOWS_DPI_AWARENESS, "permonitorv2");

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER) < 0)
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

	mPtrSDLWindow.reset(SDL_CreateWindow("Hereford",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, mScreenWidth, mScreenHeight,
		SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI));

	if (!mPtrSDLWindow)
	{
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	mPtrRenderer = std::make_unique<Renderer>(mPtrSDLWindow.get(), this, mScreenWidth, mScreenHeight);
	mPtrRenderer->SetBackgroundClearColor(Vec3(0.2f, 0.3f, 0.3f));
	mPtrRenderer->SetBackgroundClearMode(EBGClearMode::SKYBOX);
	if (!mPtrRenderer->Initialize())
	{
		printf("Renderer could not be initialized! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	

	mPtrPhysicsManager = std::make_unique<PhysicsManager>();

	mPtrAudioManager = std::make_unique<AudioManager>();
	mPtrAudioManager->CacheSound("bootcamp_ambient.wav");

	mPtrInputManager = std::make_unique<InputManager>();
	mPtrInputManager->Initialize();
	hQuitSub = mPtrInputManager->Subscribe(EInputAction::GAME_QUIT, std::bind(&GameContext::OnQuitInput, this, std::placeholders::_1), EInputState::PRESSED);

	//SDL_SetWindowGrab(pSDLWindow, SDL_TRUE);
	SDL_SetRelativeMouseMode(SDL_TRUE);

	mIsRunning = true;
	LoadStarterData();
	LoadScene("Scenes/showcase.json");
	//LoadCubeTest();

	mPtrRenderer->GroupInstancedData();

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	ImGui::StyleColorsDark();
	ImGui_ImplSDL2_InitForOpenGL(mPtrSDLWindow.get(), mPtrRenderer->GetGLContext());
	ImGui_ImplOpenGL3_Init();

	mPtrAudioManager->PlaySound("bootcamp_ambient.wav", true, false);

	

	return true;
}

void GameContext::Shutdown()
{
	mPtrInputManager->Unsubscribe(EInputAction::GAME_QUIT, hQuitSub);

	mActors.clear();

	mPtrRenderer->Shutdown();
	
	ImGui::DestroyContext();

	mPtrSDLWindow.reset();
	SDL_Quit();
}

void GameContext::RunLoop()
{

	while (mIsRunning)
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();
		
		Profiler::UpdateImGuiView();
		
		Profiler::Start("CPU", 100);
		Profiler::Start("Input", 105);
		ProcessInput();
		Profiler::Mark("Input");
		Profiler::Start("Game Logic", 110);
		UpdateGame();
		Profiler::Mark("Game Logic");
		UpdateAudio();
		CalculatePhysics();
		//DebugSceneObjects();
		DelayedActionManager::UpdateTimers(mDeltaTime);
		TimelineActionManager::UpdateTimers(mDeltaTime);
		Profiler::Mark("CPU");
		Profiler::Start("GPU", 200);
		GenerateOutput();
		Profiler::Mark("GPU");
	}
	Shutdown();
}

void GameContext::LoadStarterData()
{
	if (false)
	{
		mPtrPlayer = CreateActor<Player>(this);
		mPtrPlayer->SetPosition(Vector3(4.0f, 0.0f, 4.0f));
		mPtrRenderer->SetMainCamera(&mPtrPlayer->GetMainCamera());

		mPtrAudioManager->SetPlayerReference(mPtrPlayer);
	}
	else
	{
		auto* flyCamera = CreateActor<FlyCamera>(this);
		mPtrRenderer->SetMainCamera(&flyCamera->GetMainCamera());

		mPtrAudioManager->SetPlayerReference(flyCamera);
	}

	mPtrTestMaster = CreateActor<TestMaster>(this);
	mPtrRenderer->SetTestMaster(mPtrTestMaster);
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
			pActor = CreateActor<PlywoodWall>(this, actor["type"]);
		}
		else
		{
			auto created = ReflectionRegistry::Instance().CreateInstance(actor["className"], this);
			pActor = AddActor(std::move(created));
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

void GameContext::LoadCubeTest()
{
	for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < 100; j++)
		{
			auto* pActor = CreateActor<TestCube>(this);
			pActor->SetPosition(Vec3(i * 2, 0, j * 2));
		}
	}
}

void GameContext::ClearScene()
{
	std::erase_if(mActors, [](const auto& actor) {
		return !dynamic_cast<Player*>(actor.get());
	});
}

void GameContext::SaveScene(const std::string& sceneFilePath)
{
	using json = nlohmann::json;
	json scene;

	for (const auto& actor : mActors)
	{
		if (dynamic_cast<Player*>(actor.get()))
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
	while (SDL_PollEvent(&pollEvent))
	{
		ImGui_ImplSDL2_ProcessEvent(&pollEvent);
		switch (pollEvent.type)
		{
			// Handle events on InputMgr's side
		case SDL_CONTROLLERDEVICEADDED:
		case SDL_CONTROLLERDEVICEREMOVED:
		case SDL_MOUSEWHEEL:
			mPtrInputManager->HandleEvent(pollEvent);
			break;
		default:
			break;
		}
	}

	// Handle keyboard and mouse states
	mPtrInputManager->Poll();
}

void GameContext::UpdateGame()
{
	mCurrTimestamp = SDL_GetTicks();
	mDeltaTime = (mCurrTimestamp - mPrevTimestamp) / 1000.0f;
	mPrevTimestamp = mCurrTimestamp;

	size_t count = mActors.size();
	for (size_t i = 0; i < count; i++)
	{
		mActors[i]->Update(mDeltaTime);
	}

	std::erase_if(mActors, [](const auto& actor) {
		return actor->GetState() == EActorState::Destroy;
	});
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
			AddActor(ReflectionRegistry::Instance().CreateInstance(std::string(actorClass), this));
		}
		ImGui::TreePop();
	}
	
	ImGui::End();
}

void GameContext::OnQuitInput(EInputState state)
{
	mIsRunning = false;
}



Actor* GameContext::AddActor(std::unique_ptr<Actor> actor)
{
	if (!actor)
		return nullptr;
	Actor* raw = actor.get();
	mActors.push_back(std::move(actor));
	return raw;
}

void GameContext::RemoveActor(Actor* actor)
{
	std::erase_if(mActors, [actor](const auto& a) {
		return a.get() == actor;
	});
}
