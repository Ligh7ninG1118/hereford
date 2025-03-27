#pragma once

#include <cstdint>

class Component
{
public:
	Component(uint32_t ownerID);
	virtual void Create();
	virtual ~Component();

private:
	uint32_t mOwnerEntityID;
};

