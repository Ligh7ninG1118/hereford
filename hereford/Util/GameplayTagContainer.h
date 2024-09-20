#pragma once
#include <set>
#include <string>
#include "GameplayTag.h"

class GameplayTagContainer
{
public:
	void AddTag(const GameplayTag& tag) { mTags.insert(tag); }
	void RemoveTag(const GameplayTag& tag) { mTags.erase(tag); }

	bool HasTag(const GameplayTag& tag) const { return mTags.find(tag) != mTags.end(); }
	bool HasAny(const GameplayTagContainer& other) const 
	{
		for (const auto& tag : other.mTags)
			if (HasTag(tag))
				return true;
		return false;
	}

	bool HasAll(const GameplayTagContainer& other) const
	{
		for (const auto& tag : other.mTags)
			if (!HasTag(tag))
				return false;
		return true;
	}

private:
	std::set<GameplayTag> mTags;
};