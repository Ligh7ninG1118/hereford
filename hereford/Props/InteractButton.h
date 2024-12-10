#pragma once
#include "Core/Actor.h"
#include "Gameplay/IInteractable.h"
#include "Util/Reflection.h"
#include <memory>

class InteractButton : public Actor, public IInteractable
{
	REFLECT(InteractButton)

public:
	InteractButton(class GameContext* gameCtx);
	~InteractButton();

	void Interact() override;

private:
	std::unique_ptr<class RenderComponent> mPtrRenderComp;
	std::unique_ptr<class PhysicsComponent> mPtrPhysicsComp;
	std::unique_ptr<class AudioComponent> mPtrAudioComp;

};

