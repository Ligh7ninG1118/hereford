#include "Actor.h"
#include "GameContext.h"
#include "Component.h"
#include <algorithm>


Actor::Actor(GameContext* game)
	: 
	mGameCtx(game),
	mState(ActorState::Active),
	mPosition(Vector3::Zero),
	mScale(Vector3::One),
	mRotation(Vector3::Zero)
{
	mGameCtx->AddActor(this);
}

Actor::~Actor()
{
	//printf("Actor Destructor\n");

	// Using smart pointers to manage, no need for this
	/*for (Component* pCom : mComponents)
	{
		delete pCom;
	}*/
	mGameCtx->RemoveActor(this);
}

void Actor::Update(float deltaTime)
{
	for (Component* pCom : mComponents)
	{
		pCom->Update(deltaTime);
	}

	OnUpdate(deltaTime);
}

void Actor::ProcessInput(const std::vector<EInputState>& keyState, Uint32 mouseState, int mouseDeltaX, int mouseDeltaY)
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

void Actor::OnProcessInput(const std::vector<EInputState>& keyState, Uint32 mouseState, int mouseDeltaX, int mouseDeltaY)
{
}

void Actor::AddComponent(Component* c)
{
	mComponents.emplace_back(c);
	std::sort(mComponents.begin(), mComponents.end(), [](Component* a, Component* b) {
		return a->GetUpdateOrder() < b->GetUpdateOrder();
		});
}
