#pragma once

#include "HerefordCommon.h"


class ListenerComponent : public Component
{
public:
	ListenerComponent(uint32_t ownerID);
	~ListenerComponent();

	Vector3 GetPosition() const { return Vector3::Zero; }
	Vector3 GetForward() const { return Vector3::Zero; }

};

