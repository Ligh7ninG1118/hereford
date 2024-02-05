#pragma once

#include <SDL2/SDL_stdinc.h>
#include <vector>

class Renderer
{
public:
	Renderer(SDL_Window* sdlWindow, int width, int height);

	bool Initialize();
	void Shutdown();
	void Render();

private:
	SDL_Window* m_pSDLWindowContext;

	SDL_GLContext m_pGLContext;

	int m_ScreenWidth;
	int m_ScreenHeight;

	std::vector<class RenderComponent> m_RenderCompons;
};

