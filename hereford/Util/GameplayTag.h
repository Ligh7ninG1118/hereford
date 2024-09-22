#pragma once
#include <string>

class GameplayTag
{
public:
	GameplayTag(const std::string& tagName) : mName(tagName) { }

	bool operator==(const GameplayTag& other) const
	{
		return mName == other.mName;
	}

	bool operator<(const GameplayTag& other) const
	{
		return mName.compare(other.mName);
	}

	std::string GetName() const { return mName; }

private:
	std::string mName;
};