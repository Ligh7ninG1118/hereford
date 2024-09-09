#include "AnimatedRenderComponent.h"
#include "Core/Actor.h"
#include "Gameplay/CameraComponent.h"


AnimatedRenderComponent::AnimatedRenderComponent(Actor* owner, Renderer& rendererRef)
	:
	RenderComponent(owner, rendererRef),
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
		rot.mX = -camRot.mY;
		rot.mZ = -camRot.mX;
	}
	else
	{
		rot = mOwner->GetRotation();
	}
	mat.Rotate(mRotateOffset);
	mat.Rotate(rot);
	mat.Translate(mTranslateOffset);
	mat.Translate(mOwner->GetPosition());

	return mat;
}

