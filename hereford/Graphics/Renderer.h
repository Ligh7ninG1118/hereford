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

	int m_ScreenWidth;
	int m_ScreenHeight;

	std::vector<class RenderComponent*> mRenderComponents;

	//std::vector<class RenderComponent> m_RenderCompons;
	friend class RenderComponent;
	// Adds component to Actor (this is automatically called
	// in the component constructor)
	void AddRenderComponent(class RenderComponent* c);
};

