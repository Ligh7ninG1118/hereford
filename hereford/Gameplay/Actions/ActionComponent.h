#pragma once
#include "Action.h"
#include "Core/Component.h"
#include "Util/GameplayTagContainer.h"
#include <set>
#include <string>


class ActionComponent : public Component
{
public:
	ActionComponent(class Actor* owner);
	~ActionComponent();

	void AddAction(class Action& inAction);
	void RemoveAction(const class Action& inAction);

	bool HasActionByName(const std::string& actionName);
	bool StartActionByName(const std::string& actionName);
	bool StopActionByName(const std::string& actionName);

	void AddActiveTags(const GameplayTagContainer& tags) { mActiveGameplayTags.AddTags(tags); }
	void RemoveActiveTags(const GameplayTagContainer& tags) { mActiveGameplayTags.RemoveTags(tags); }
	const GameplayTagContainer& GetActiveGameplayTags() const { return mActiveGameplayTags; }

protected:
	std::set<Action> mActions;
	GameplayTagContainer mActiveGameplayTags;
};

