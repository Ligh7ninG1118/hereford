#pragma once
#include "Util/GameplayTagContainer.h"

class Action
{
public:
	Action(std::string actionName);
	~Action();

	bool IsRunning() const { return mIsRunning; }
	void SetOwner(class ActionComponent* owner) { mOwner = owner; }

	std::string GetActionName() const { return mActionName; }

	void AddGrantsTag(const GameplayTag& tag) { mGrantsTags.AddTag(tag); }
	void AddBlockeTag(const GameplayTag& tag) { mBlockedTags.AddTag(tag); }

	bool CanStart() const;
	void StartAction();
	void StopAction();

	bool operator==(const Action& other) const
	{
		return mActionName == other.mActionName;
	}
	bool operator<(const Action& other) const
	{
		return mActionName < other.mActionName;
	}

protected:
	class ActionComponent* mOwner;

	GameplayTagContainer mGrantsTags;
	GameplayTagContainer mBlockedTags;

	std::string mActionName;
	bool mIsRunning;
};

