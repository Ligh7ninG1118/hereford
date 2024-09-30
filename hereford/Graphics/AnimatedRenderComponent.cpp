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
	Vec3 ownPos = mOwner->GetPosition();
	Vec3 actorFwd = mOwner->GetForward();
	Vec3 actorRight = -actorFwd.Cross(Vec3(0.0f, 1.0f, 0.0f)).normalized();
	Vec3 actorUp = -actorRight.Cross(actorFwd);

	if (mControlByFPCam)
	{
		Vec3 camRot = mPtrCamera->GetRotation();
		rot.mX = -camRot.mX;
		rot.mZ = -camRot.mY;

		actorFwd = mPtrCamera->GetFrontVector();
		actorRight = -actorFwd.Cross(Vec3(0.0f, 1.0f, 0.0f)).normalized();
		actorUp = -actorRight.Cross(actorFwd);
		ownPos += mPtrCamera->GetPositionOffset();
	}
	else
	{
		rot = mOwner->GetRotation();
	}
	mat.Rotate(mRotateOffset + rot);

	Vec3 actualOffset = Vec3::Zero;
	actualOffset += mTranslateOffset.mX * actorFwd;
	actualOffset += mTranslateOffset.mY * actorUp;
	actualOffset += mTranslateOffset.mZ * actorRight;

	mat.Translate(actualOffset + ownPos);

	return mat;
}

