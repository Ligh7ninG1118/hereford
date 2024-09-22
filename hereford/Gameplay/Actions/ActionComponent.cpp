#include "ActionComponent.h"
#include "Core/Actor.h"

ActionComponent::ActionComponent(Actor* owner)
	: Component(owner)
{
}

ActionComponent::~ActionComponent()
{
}

void ActionComponent::AddAction(Action& inAction)
{
	inAction.SetOwner(this);
	mActions.insert(inAction);
}

void ActionComponent::RemoveAction(const Action& inAction)
{
	mActions.erase(inAction);
}

bool ActionComponent::HasActionByName(const std::string& actionName)
{
	Action targetAction(actionName);

	if (mActions.find(targetAction) != mActions.end())
		return true;
	return false;
}

bool ActionComponent::StartActionByName(const std::string& actionName)
{
	//TODO: Optimize this: different data strucutre?
	auto it = std::find_if(mActions.begin(), mActions.end(), [&actionName](const Action& action) {
		return action.GetActionName() == actionName;
		});

	if (it == mActions.end())
	{
		//printf("ActionComponent::StartActionByName(): Action [%s] doesn't exist in component\n", actionName.c_str());
		return false;
	}

	Action& targetAction = const_cast<Action&>(*it);


	if (targetAction.IsRunning())
	{
		//printf("ActionComponent::StartActionByName(): Action [%s] is already running\n", targetAction.GetActionName().c_str());
		return false;
	}

	if (!targetAction.CanStart())
	{
		//printf("ActionComponent::StartActionByName(): Action [%s] blocked by other tags\n", targetAction.GetActionName().c_str());
		return false;
	}

	//printf("ActionComponent::StartActionByName(): Action [%s] started!\n", targetAction.GetActionName().c_str());
	targetAction.StartAction();
	return true;
}

bool ActionComponent::StopActionByName(const std::string& actionName)
{
	auto it = std::find_if(mActions.begin(), mActions.end(), [&actionName](const Action& action) {
		return action.GetActionName() == actionName;
		});

	if (it == mActions.end())
	{
		//printf("ActionComponent::StopActionByName(): Action [%s] doesn't exist in component\n", actionName.c_str());
		return false;
	}

	Action& targetAction = const_cast<Action&>(*it);


	if (!targetAction.IsRunning())
	{
		//printf("ActionComponent::StopActionByName(): Action [%s] has already stopped\n", targetAction.GetActionName().c_str());
		return false;
	}
	//printf("ActionComponent::StopActionByName(): Action [%s] stopped\n", targetAction.GetActionName().c_str());
	targetAction.StopAction();
	return true;
}
