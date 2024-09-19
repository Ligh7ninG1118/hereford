#include "AnimatedRenderComponent.h"
#include "Core/Actor.h"
#include "Gameplay/CameraComponent.h"


AnimatedRenderComponent::AnimatedRenderComponent(Actor* owner, Renderer& rendererRef, ERenderLayer renderLayer)
	:
	RenderComponent(owner, rendererRef, renderLayer),
	mPtrAnimator(nullptr),
	mPtrCamera(nullptr),
	mControlByFPCam(false)
{
}

AnimatedRenderComponent::~AnimatedRenderComponent()
{
}

Mat4 AnimatedRenderComponent::GetModelMatrix() const
{
	Mat4 mat = Mat4::Identity;
	mat.Scale(mScaleOffset);
	mat.Scale(mOwner->GetScale());

	Vec3 rot = Vec3::Zero;
	if (mControlByFPCam)
	{
		Vec3 camRot = mPtrCamera->GetRotation();
		rot.mX = -camRot.mX;
		rot.mZ = -camRot.mY;
	}
	else
	{
		rot = mOwner->GetRotation();
	}
	mat.Rotate(mRotateOffset + rot);
	mat.Translate(mTranslateOffset + mOwner->GetPosition());

	return mat;
}

