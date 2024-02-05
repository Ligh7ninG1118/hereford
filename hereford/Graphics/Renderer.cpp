#include "Renderer.h"
#include <glad/glad.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <stdio.h>
#include <string>

Renderer::Renderer(SDL_Window* sdlWindow, int width, int height)
	:
	m_pSDLWindowContext(sdlWindow),
	m_ScreenWidth(width),
	m_ScreenHeight(height)
{
}

bool Renderer::Initialize()
{
	m_pGLContext = SDL_GL_CreateContext(m_pSDLWindowContext);
	if (m_pGLContext == nullptr)
	{
		printf("Renderer::Initialize(): Main Context could not be created! SDL_Error: %s\n", SDL_GetError());
		return false;
	}


	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
	{
		printf("Renderer::Initialize(): Failed to initialize GLAD\n");
		return false;
	}

	printf("Renderer::Initialize(): OpenGL loaded\n");
	printf("Renderer::Initialize(): Vendor:   %s\n", glGetString(GL_VENDOR));
	printf("Renderer::Initialize(): Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Renderer::Initialize(): Version:  %s\n", glGetString(GL_VERSION));

	SDL_GL_SetSwapInterval(1);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glViewport(0, 0, m_ScreenWidth, m_ScreenHeight);
	glClearColor(0.0f, 0.5f, 1.0f, 0.0f);
}

void Renderer::Shutdown()
{
}

void Renderer::Render()
{
}
