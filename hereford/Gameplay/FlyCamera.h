#pragma once
#include "Core/Actor.h"
#include "Util/Reflection.h"
#include <memory>
#include "Input/InputCommons.h"

class FlyCamera : public Actor
{
	REFLECT(FlyCamera)
public:
	FlyCamera(class GameContext* gameCtx);
	~FlyCamera();

	void OnUpdate(float deltaTime) override;

	class CameraComponent& GetMainCamera() const { return *mPtrCameraComp.get(); }
private:
	void ProcessMovement(float deltaTime);

	std::unique_ptr<class CameraComponent> mPtrCameraComp;

};

