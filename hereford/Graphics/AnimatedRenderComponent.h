#pragma once
#include "RenderComponent.h"
#include "Asset/Model.h"
#include <memory>

class AnimatedRenderComponent : public RenderComponent
{
public:
	AnimatedRenderComponent(class Actor* owner, class Renderer& rendererRef);
	~AnimatedRenderComponent();

	inline std::weak_ptr<class Model> GetModel() const { return mPtrModel; }
	inline void SetModel(std::shared_ptr<class Model> inModel) { mPtrModel = inModel; }

	inline const std::vector<Mesh>& GetMeshes() const { return mPtrModel->mMeshes; }

	inline class Animator* GetAnimator() const { return mPtrAnimator; }
	inline void SetAnimator(class Animator* inPtr) { mPtrAnimator = inPtr; }

	inline class CameraComponent* GetCamera() const { return mPtrCamera; }
	inline void SetCamera(class CameraComponent* inPtr) { mPtrCamera = inPtr; mControlByFPCam = true; }

	Mat4 GetModelMatrix() const override;


private:
	std::shared_ptr<Model> mPtrModel;
	class Animator* mPtrAnimator;
	class CameraComponent* mPtrCamera;

	bool mControlByFPCam;
};

