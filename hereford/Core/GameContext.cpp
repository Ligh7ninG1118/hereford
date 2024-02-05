#include "GameContext.h"

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

	pSDLWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, m_ScreenWidth, m_ScreenHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	if (pSDLWindow == nullptr)
	{
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	

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
}

void GameContext::ProcessInput()
{
}

void GameContext::UpdateGame()
{
}

void GameContext::GenerateOutput()
{
}

void GameContext::AddActor(Actor* actor)
{
}

void GameContext::RemoveActor(Actor* actor)
{
}
