#pragma once

#include "Shader.h"
#include "../Gameplay/CameraComponent.h"
#include <SDL2/SDL.h>
#include <vector>

class Renderer
{
public:
	Renderer(SDL_Window* sdlWindow, class GameContext* gameContext, int width, int height);
	~Renderer();
	bool Initialize();
	void Shutdown();
	void Render(float deltaTime);

	void SetMainCamera(CameraComponent* pMainCam);

private:
	SDL_Window* m_pSDLWindowContext;
	SDL_GLContext m_pGLContext;
	CameraComponent* m_pMainCamera;
	class GameContext* m_pGameContext;

	int m_ScreenWidth;
	int m_ScreenHeight;

	Uint32 debugShader;

	std::vector<class RenderComponent*> mRenderComponents;
	std::vector<class LightComponent*> mLightComponents;


	//std::vector<class RenderComponent> m_RenderCompons;
	friend class RenderComponent;
	// Adds component to Actor (this is automatically called
	// in the component constructor)
	void AddRenderComponent(class RenderComponent* c);
	void RemoveRenderComponent(class RenderComponent* c);

	friend class LightComponent;
	void AddLightComponent(class LightComponent* c);
	void RemoveLightComponent(class LightComponent* c);
};

