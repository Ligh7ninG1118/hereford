#pragma once

#include "Gameplay/CameraComponent.h"
#include "RenderComponent.h"

#include "Asset/Model.h"
#include "Asset/Shader.h"
#include <SDL2/SDL.h>
#include <vector>
#include <memory>
#include <unordered_map>


enum class EBGClearMode
{
	UNINITIALIZED = 0,
	PURE_COLOR_ONLY,
	SKYBOX,
};

struct Character
{
	Uint32 mTextureID;
	Vec2 mSize;
	Vec2 mBearing;
	Uint32 mAdvance;
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
	SDL_GLContext GetGLContext() const { return mGLContext; }

	void AddDebugLines(Vec3 startPos, Vec3 endPos);

	void SetTestMaster(class TestMaster* pMaster) { mTestMaster = pMaster; }

protected:
	std::map<ERenderLayer, std::vector<class RenderComponent*>> FrustumCullingPass();
	bool IsWithinFrustum(const struct PhysicsPrimitive& boundingVolume, const Mat4& modelMatrix, const std::vector<Plane>& planes);
	std::vector<Plane> GenerateFrustum (const CameraComponent& cam) const;

private:
	SDL_Window* mPtrSDLWindowContext;
	SDL_GLContext mGLContext;
	CameraComponent* mPtrMainCamera;
	class GameContext* mPtrGameContext;

	int mScreenWidth;
	int mScreenHeight;
	std::shared_ptr<Shader> debugLineShader;

	std::shared_ptr<Shader> skyboxShader;
	Uint32 skyboxVAOID;
	Uint32 skyboxTexID;
	Uint32 irraTexID;
	Uint32 prefilterTexID;
	Uint32 brdfLUTTexture;
	Uint32 captureFBO;
	std::shared_ptr<Shader> irraShader;
	std::shared_ptr<Shader> prefilterShader;
	std::shared_ptr<Shader> brdfShader;


	std::shared_ptr<Shader> textShader;
	Uint32 textVAO;
	Uint32 textVBO;
	std::map<char, Character> Characters;

	class TestMaster* mTestMaster;

	std::map<ERenderLayer, std::vector<class RenderComponent*>> mRenderComponentMap;

	std::vector<class LightComponent*> mLightComponents;

	std::vector<class UIElement*> mUIElements;

	std::vector<uint32> mDebugLines;

	friend class RenderComponent;
	void AddRenderComponent(class RenderComponent* c);
	void RemoveRenderComponent(class RenderComponent* c);

	friend class LightComponent;
	void AddLightComponent(class LightComponent* c);
	void RemoveLightComponent(class LightComponent* c);

	friend class UIElement;
	void AddUIElement(class UIElement* ui);
	void RemoveUIElement(class UIElement* ui);
};

