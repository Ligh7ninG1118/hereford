#pragma once
#include "Action.h"
#include "Core/Component.h"
#include "Util/GameplayTagContainer.h"
#include <vector>


class ActionComponent : public Component
{
public:
	ActionComponent();
	~ActionComponent();

	void AddAction();
	void RemoveAction();
	bool HasAction();


protected:
	std::vector<Action> mActions;
	GameplayTagContainer mActiveGameplayTags;
};

