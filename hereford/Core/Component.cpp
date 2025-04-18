#include "Component.h"
#include "Actor.h"
#include "GameContext.h"

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

GameContext* Component::GetGameContext() const
{
	return GetOwner()->GetGameContext();
}
