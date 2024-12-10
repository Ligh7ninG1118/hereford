#pragma once
#include "Core/Actor.h"
#include "Util/GameEvent.h"

#include "Util/Reflection.h"
#include <memory>

enum class ETestState
{
	START,
	RUNNING,
	RESULT
};

class TestMaster : public Actor
{
	REFLECT(TestMaster)
public:
	TestMaster(class GameContext* gameCtx);
	~TestMaster();

	void OnUpdate(float deltaTime) override;

	std::string GetOutputString();

private:
	void InteractBtnListner(EventOnInteractButtonPressed inEvent);
	GameEvent::Subscription<EventOnInteractButtonPressed>* mPtrBtnEvent;

	float mTimer;
	ETestState mTestState;
};

