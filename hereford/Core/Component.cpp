#include "Component.h"
#include "Actor.h"

OldComponent::OldComponent(Actor* owner, int updateOrder)
	: mOwner(owner), mUpdateOrder(updateOrder)
{
	mOwner->AddComponent(this);
}

OldComponent::~OldComponent()
{
}

void OldComponent::Update(float deltaTime)
{
}

void OldComponent::ProcessInput(const std::vector<EInputState>& keyState, Uint32 mouseState, int mouseDeltaX, int mouseDeltaY)
{
}

Game* OldComponent::GetGame() const
{
	return nullptr;
}
