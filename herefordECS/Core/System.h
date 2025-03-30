#pragma once

#include <cstdint>


class System
{
public:
	System(uint8_t updateOrder = 100);
	~System();

	virtual void Update(float deltaTime);

private:
	uint8_t mUpdateOrder;
};

