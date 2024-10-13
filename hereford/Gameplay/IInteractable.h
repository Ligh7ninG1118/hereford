#pragma once
#include <string>

struct IInteractable
{
	IInteractable() {}
	~IInteractable() {}
	virtual std::string GetInteractPrompt() const { return "Interact"; }
	virtual void Interact() = 0;
};