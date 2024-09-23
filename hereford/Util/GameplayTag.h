#pragma once
#include <string>

enum class EActionType
{
	RESERVED_DEFAULT = 0,
	RELOADING,
	CROUCHING,
	SPRINTING,
};


class GameplayTag
{
public:
	GameplayTag(const EActionType& action) : mAction(action) { }

	bool operator==(const GameplayTag& other) const
	{
		return mAction == other.mAction;
	}

	bool operator<(const GameplayTag& other) const
	{
		return mAction < other.mAction;
	}

	EActionType GetAction() const { return mAction; }

private:
	EActionType mAction;
};