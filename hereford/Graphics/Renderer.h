#pragma once

#include "Shader.h"
#include <SDL2/SDL.h>
#include <vector>

class Renderer
{
public:
	Renderer(SDL_Window* sdlWindow, int width, int height);

	bool Initialize();
	void Shutdown();
	void Render(float deltaTime);

private:
	SDL_Window* m_pSDLWindowContext;
	SDL_GLContext m_pGLContext;

	Shader stdShader;
	unsigned int VAO, VBO;

	int m_ScreenWidth;
	int m_ScreenHeight;

	//std::vector<class RenderComponent> m_RenderCompons;
};

