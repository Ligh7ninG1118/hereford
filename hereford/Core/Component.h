#pragma once

#include "Util/Enums.h"
#include <SDL2/SDL_stdinc.h>
#include <vector>

class Component
{
public:
	// Constructor
	Component(class Actor* owner, int updateOrder = 100);
	// Destructor
	virtual ~Component();
	// Update this component by delta time
	virtual void Update(float deltaTime);
	// Process input for this component (if needed)
	virtual void ProcessInput(const std::vector<EInputState>& keyState, Uint32 mouseState, int mouseDeltaX, int mouseDeltaY);

	int GetUpdateOrder() const { return mUpdateOrder; }
	class Actor* GetOwner() const { return mOwner; }
	class Game* GetGame() const;

protected:
	// Owning actor
	class Actor* mOwner;
	// Update order
	int mUpdateOrder;
};

