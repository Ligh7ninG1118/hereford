#pragma once
#include "Core/Actor.h"
#include "Util/Reflection.h"
#include <memory>


class FlyCamera : public Actor
{
	REFLECT(FlyCamera)
public:
	FlyCamera(class GameContext* gameCtx);
	~FlyCamera();

	void OnUpdate(float deltaTime) override;
	void OnProcessInput(const std::vector<EInputState>& keyState, Uint32 mouseState, int mouseDeltaX, int mouseDeltaY) override;

	class CameraComponent& GetMainCamera() const { return *mPtrCameraComp.get(); }
private:
	void ProcessMovement(float deltaTime);
	std::unique_ptr<class CameraComponent> mPtrCameraComp;
	bool hasMovementInput = false;
	Vector3 inputMoveDir;
};

