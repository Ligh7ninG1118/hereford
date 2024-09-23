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

	Vec3 actorFwd = mOwner->GetForward();
	Vec3 actorRight = -actorFwd.Cross(Vec3(0.0f, 1.0f, 0.0f)).normalized();
	Vec3 actorUp = Vec3(0.0f, 1.0f, 0.0f);

	float x = mTranslateOffset.Dot(actorFwd);
	float y = mTranslateOffset.Dot(actorUp);
	float z = mTranslateOffset.Dot(actorRight);

	Vec3 actualOffset = Vec3(x, y, z);

	mat.Translate(actualOffset + mOwner->GetPosition());

	return mat;
}

