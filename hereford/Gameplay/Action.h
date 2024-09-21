#pragma once
#include "Util/GameplayTagContainer.h"

class Action
{
public:
	Action();
	~Action();

	bool IsRunning() const;
	bool CanStart() const;
	void StartAction();
	void StopAction();


protected:
	GameplayTagContainer mGrantsTags;
	GameplayTagContainer mBlockedTags;

	float mTimeStarted;
};

