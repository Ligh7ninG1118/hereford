#pragma once

struct IInteractable
{
	IInteractable() {}
	~IInteractable() {}

	virtual void Interact() = 0;
};