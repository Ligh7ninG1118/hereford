#pragma once

#include "Shader.h"
#include "../Gameplay/CameraComponent.h"
#include <SDL2/SDL.h>
#include <vector>

class Renderer
{
public:
	Renderer(SDL_Window* sdlWindow, int width, int height);

	bool Initialize();
	void Shutdown();
	void Render(float deltaTime);

	void SetMainCamera(CameraComponent* pMainCam);

private:
	SDL_Window* m_pSDLWindowContext;
	SDL_GLContext m_pGLContext;
	CameraComponent* m_pMainCamera;

	Shader stdShader;
	unsigned int VAO, VBO;

	int m_ScreenWidth;
	int m_ScreenHeight;

	//std::vector<class RenderComponent> m_RenderCompons;
};

