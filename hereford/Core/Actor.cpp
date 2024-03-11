#include "Actor.h"
#include "GameContext.h"
#include "Component.h"
#include <algorithm>

Actor::Actor(GameContext* game)
	: 
	mGame(game),
	mState(ActorState::Active),
	mPosition(Vector3::Zero),
	mScale(Vector3::One),
	mRotation(Vector3::Zero)
{
	mGame->AddActor(this);
}

Actor::~Actor()
{
	for (Component* pCom : mComponents)
	{
		delete pCom;
	}
	mGame->RemoveActor(this);
}

void Actor::Update(float deltaTime)
{
	for (Component* pCom : mComponents)
	{
		pCom->Update(deltaTime);
	}

	OnUpdate(deltaTime);
}

void Actor::ProcessInput(const Uint8* keyState, Uint32 mouseState, int mouseDeltaX, int mouseDeltaY)
{
	if (mState != ActorState::Active)
		return;

	for (Component* pCom : mComponents)
	{
		pCom->ProcessInput(keyState, mouseState, mouseDeltaX, mouseDeltaY);
	}

	OnProcessInput(keyState, mouseState, mouseDeltaX, mouseDeltaY);
}

void Actor::OnUpdate(float deltaTime)
{
}

void Actor::OnProcessInput(const Uint8* keyState, Uint32 mouseState, int mouseDeltaX, int mouseDeltaY)
{
}

void Actor::AddComponent(Component* c)
{
	mComponents.emplace_back(c);
	std::sort(mComponents.begin(), mComponents.end(), [](Component* a, Component* b) {
		return a->GetUpdateOrder() < b->GetUpdateOrder();
		});
}
