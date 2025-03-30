#pragma once


class System
{
public:
	System(int updateOrder = 100);
	~System();

	virtual void Update(float deltaTime);

private:
	int mUpdateOrder;
};

