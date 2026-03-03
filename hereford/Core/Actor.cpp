#include "Actor.h"
#include "GameContext.h"
#include "Component.h"
#include <algorithm>


Actor::Actor(GameContext* game)
	: 
	mGameCtx(game),
	mState(EActorState::Enabled),
	mPosition(Vector3::Zero),
	mScale(Vector3::One),
	mRotation(Vector3::Zero)
{
}

Actor::~Actor() = default;

void Actor::Update(float deltaTime)
{
	for (Component* pCom : mComponents)
	{
		if(pCom->GetState() == EComponentState::Enabled)
			pCom->Update(deltaTime);
	}

	OnUpdate(deltaTime);
}

void Actor::SetState(EActorState state, bool setComponents)
{
	mState = state;
	
	if (setComponents)
	{
		EComponentState cState = state == EActorState::Enabled ? EComponentState::Enabled : EComponentState::Disabled;
		for (Component* pCom : mComponents)
		{
			pCom->SetState(cState);
		}
	}
}

void Actor::OnUpdate(float deltaTime)
{
}

void Actor::AddComponent(Component* c)
{
	mComponents.emplace_back(c);
	std::sort(mComponents.begin(), mComponents.end(), [](Component* a, Component* b) {
		return a->GetUpdateOrder() < b->GetUpdateOrder();
		});
}
