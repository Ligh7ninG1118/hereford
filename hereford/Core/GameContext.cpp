#include "Core/GameContext.h"
#include "Core/Actor.h"
#include "Gameplay/Player.h"
#include "Props/TestCube.h"
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
	: m_ScreenWidth(1920), m_ScreenHeight(1080)
{
	// Enough for most input key usage
	mPrevKeyStates.resize(70);
	mPrevMouseStates = EMouseState::LMB_NOT_PRESSED | EMouseState::MMB_NOT_PRESSED | EMouseState::RMB_NOT_PRESSED;
}

GameContext::GameContext(int width, int height)
	: m_ScreenWidth(width), m_ScreenHeight(height)
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

	pSDLWindow = SDL_CreateWindow("Hereford",
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
	//SDL_SetRelativeMouseMode(SDL_TRUE);

	isRunning = true;
	LoadData();
	LoadScene("Scenes/test.json");

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
	ImGui::StyleColorsDark();
	ImGui_ImplSDL2_InitForOpenGL(pSDLWindow, pRenderer->GetGLContext());
	ImGui_ImplOpenGL3_Init();

	return true;
}

void GameContext::Shutdown()
{
	while (!mActors.empty())
	{
		delete mActors.back();
	}

	
	/*ImGui::DestroyContext();
	ImGui_ImplSDL2_Shutdown();*/

	SDL_DestroyWindow(pSDLWindow);
	pSDLWindow = nullptr;
	SDL_Quit();
}

void GameContext::RunLoop()
{
	while (isRunning)
	{
		bool show_demo_window = true;
		bool show_another_window = false;
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();

		// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
		{
			static float f = 0.0f;
			static int counter = 0;

			ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

			ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
			ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
			ImGui::Checkbox("Another Window", &show_another_window);

			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

			if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
				counter++;
			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);

			//ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
			ImGui::End();
		}

		// 3. Show another simple window.
		if (show_another_window)
		{
			ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
			ImGui::Text("Hello from another window!");
			if (ImGui::Button("Close Me"))
				show_another_window = false;
			ImGui::End();
		}


		Uint32 timestampStart = SDL_GetTicks();
		ProcessInput();
		Uint32 timestampAfterInput = SDL_GetTicks();
		//printf("Process Input: %d ms \n", timestampAfterInput - timestampStart);
		UpdateGame();
		DelayedActionManager::UpdateTimers(deltaTime);
		Uint32 timestampUpdate = SDL_GetTicks();
		cpuTime = timestampUpdate - timestampStart;
		//printf("Update Game: %d ms \n", timestampUpdate - timestampAfterInput);
		CalculatePhysics();
		GenerateOutput();
		Uint32 timestampRender = SDL_GetTicks();
		gpuTime = timestampRender - timestampUpdate;
		//printf("Render: %d ms \n", timestampRender - timestampUpdate);
		// Rendering
		// (Your code clears your framebuffer, renders your other stuff etc.)
		
		// (Your code calls SDL_GL_SwapWindow() etc.)
		
	}
	Shutdown();
}

void GameContext::LoadData()
{
	mPtrPlayer = new Player(this);
	pRenderer->SetMainCamera(&mPtrPlayer->GetMainCamera());

	for (int i = 0; i < 1; i++)
	{
		TestCube* cubeActor = new TestCube(this);
		//cubeActor->SetPosition(Vec3(Random::Range(5.0f, 15.0f), Random::Range(-2.0f, 2.0f), Random::Range(-5.0f, 5.0f) ));
		cubeActor->SetPosition(Vec3(5.0f, 1.8f, 0.0f));

	}

	/*NPC* npc = new NPC(this);
	npc->SetPosition(Vec3(0.0f, 0.0f, -2.0f));
	npc->SetRotation(Vec3(-90.0f, 0.0f, 0.0f));*/


	/*LightBulb* lightBulb = new LightBulb(this);
	lightBulb->SetPosition(Vec3(0.0f, 5.0f, 0.0f));

	LightBulb* lightBulb2 = new LightBulb(this);
	lightBulb2->SetPosition(Vec3(0.0f, -2.0f, 0.0f));*/

}

void GameContext::LoadScene(const std::string& sceneFilePath)
{
	/*using json = nlohmann::json;


	std::ifstream file(sceneFilePath.c_str());
	json scene;
	file >> scene;
	file.close();
*/


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
			isRunning = false;
			break;
		default:
			break;
		}
	}

	const Uint8* rawKeyState = SDL_GetKeyboardState(nullptr);
	if (rawKeyState[SDL_SCANCODE_ESCAPE])
		isRunning = false;

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

void GameContext::CalculatePhysics()
{
	pPhysicsManager->UpdatePhysics(deltaTime);

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
