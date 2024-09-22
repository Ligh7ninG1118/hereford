#include "Action.h"
#include "ActionComponent.h"

Action::Action(std::string actionName)
	: mOwner(nullptr),
	mIsRunning(false),
	mActionName(actionName)
{
}

Action::~Action()
{
}

bool Action::CanStart() const
{
	const auto& activeTags = mOwner->GetActiveGameplayTags();
	if(activeTags.HasAny(mBlockedTags))
		return false;
	return true;
}

void Action::StartAction()
{
	mIsRunning = true;
	mOwner->AddActiveTags(mGrantsTags);
}

void Action::StopAction()
{
	mIsRunning = false;
	mOwner->RemoveActiveTags(mGrantsTags);
}
