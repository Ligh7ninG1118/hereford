#pragma once

#include "Gameplay/CameraComponent.h"

#include "Asset/Model.h"
#include "Asset/Shader.h"
#include <SDL2/SDL.h>
#include <vector>
#include <memory>


enum class EBGClearMode
{
	UNINITIALIZED = 0,
	PURE_COLOR_ONLY,
	SKYBOX,
};


class Renderer : public std::enable_shared_from_this<Renderer>
{
public:
	Renderer(SDL_Window* sdlWindow, class GameContext* gameContext, int width, int height);
	~Renderer();
	bool Initialize();
	void Shutdown();
	void Render(float deltaTime);

	void SetMainCamera(CameraComponent* pMainCam);

	Vec2 GetScreenDimension() const { return Vec2(mScreenWidth, mScreenHeight); }

private:
	// TODO: remove underscore
	SDL_Window* mPtrSDLWindowContext;
	SDL_GLContext mPtrGLContext;
	CameraComponent* mPtrMainCamera;
	class GameContext* mPtrGameContext;

	int mScreenWidth;
	int mScreenHeight;

	Uint32 debugShaderID;
	std::shared_ptr<Shader> testShader;
	Uint32 crosshairVAOID;

	std::shared_ptr<Shader> skyboxShader;
	class Animation* testAnimation;
	class Animator* gunAnimator;
	class AnimationStateMachine* gunASM;
	Uint32 skyboxVAOID;
	Uint32 skyboxTexID;

	std::shared_ptr<Model> gunModel;

	std::vector<class RenderComponent*> mRenderComponents;
	std::vector<class LightComponent*> mLightComponents;

	std::vector<class UIElement*> mUIElements;

	class UIAmmoIndicator* uiAmmo;

	//std::vector<class RenderComponent> m_RenderCompons;
	friend class RenderComponent;
	// Adds component to Actor (this is automatically called
	// in the component constructor)
	void AddRenderComponent(class RenderComponent* c);
	void RemoveRenderComponent(class RenderComponent* c);

	friend class LightComponent;
	void AddLightComponent(class LightComponent* c);
	void RemoveLightComponent(class LightComponent* c);

	friend class UIElement;
	void AddUIElement(class UIElement* ui);
	void RemoveUIElement(class UIElement* ui);
};

