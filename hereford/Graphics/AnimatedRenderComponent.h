#pragma once
#include "RenderComponent.h"
#include "Asset/Model.h"
#include <memory>

class AnimatedRenderComponent : public RenderComponent
{
public:
	AnimatedRenderComponent(class Actor* owner, class Renderer& rendererRef, ERenderLayer renderLayer = ERenderLayer::Default);
	~AnimatedRenderComponent();

	inline class Animator* GetAnimator() const { return mPtrAnimator; }
	inline void SetAnimator(class Animator* inPtr) { mPtrAnimator = inPtr; }

	inline void SetCamera(class CameraComponent* inPtr) { mPtrCamera = inPtr; mControlByFPCam = true; }

	Mat4 GetModelMatrix() const override;


private:
	class Animator* mPtrAnimator;
	class CameraComponent* mPtrCamera;

	bool mControlByFPCam;
};

