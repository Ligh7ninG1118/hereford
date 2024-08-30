#pragma once

struct EventAnimFinished
{
	int mAnimIndex;

	EventAnimFinished(int animIndex)
		: mAnimIndex(animIndex)
	{}
};
