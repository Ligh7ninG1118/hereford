#include "TestMaster.h"
#include <format>


TestMaster::TestMaster(GameContext* gameCtx)
	:
	Actor(gameCtx)
{
	mPtrBtnEvent = GameEvent::Subscribe<EventOnInteractButtonPressed>(std::bind(&TestMaster::InteractBtnListner, this, std::placeholders::_1));
	mTimer = 0.0f;
	mTestState = ETestState::START;
}

TestMaster::~TestMaster()
{
	GameEvent::Unsubscribe(mPtrBtnEvent);
	delete mPtrBtnEvent;
}

void TestMaster::OnUpdate(float deltaTime)
{
	if (mTestState == ETestState::RUNNING)
	{
		mTimer += deltaTime;
	}
}

std::string TestMaster::GetOutputString()
{
	std::string result;

	switch (mTestState)
	{
	case ETestState::START:
		result = "Interact with Button to Start";
		break;
	case ETestState::RUNNING:
		result = std::format("Current Time: {:.1f}", mTimer);
		break;
	case ETestState::RESULT:
		result = std::format("Final Time: {:.1f}", mTimer);
		break;
	}

	return result;
}

void TestMaster::InteractBtnListner(EventOnInteractButtonPressed inEvent)
{
	switch (mTestState)
	{ 
	case ETestState::START:
	case ETestState::RESULT:
		mTestState = ETestState::RUNNING;
		mTimer = 0.0f;
		break;
	case ETestState::RUNNING:
		mTestState = ETestState::RESULT;
		break;
	}	
}
