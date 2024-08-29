#pragma once

struct EventTest
{
	int mX;

	EventTest(int inX)
		: mX(inX)
	{}
};

struct EventTestA
{
	bool mX;
	EventTestA(bool inX)
		: mX(inX)
	{}
};