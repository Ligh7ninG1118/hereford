#include "Component.h"
#include "Actor.h"

Component::Component(Actor* owner, int updateOrder)
	: mOwner(owner), mUpdateOrder(updateOrder)
{
	mOwner->AddComponent(this);
}

Component::~Component()
{
}

void Component::Update(float deltaTime)
{
}

void Component::ProcessInput(const Uint8* keyState, Uint32 mouseState, int mouseDeltaX, int mouseDeltaY)
{
}

Game* Component::GetGame() const
{
	return nullptr;
}
