#pragma once
#include <cstdint>
#include <vector>

class Component;

class Entity
{
public:
	Entity(uint32_t inID);
	~Entity();

	uint32_t GetEntityID() const { return mEntityID; }

	void AddComponent(Component* inCompPtr);

private:
	const uint32_t mEntityID;
	std::vector<Component*> mComponents;
};