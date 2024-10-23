#pragma once
#include "Core/Actor.h"
#include "Util/Reflection.h"

class ShowcaseModel : public Actor
{
	REFLECT(ShowcaseModel)

public:
	ShowcaseModel(class GameContext* gameCtx);
	~ShowcaseModel() {}

	class RenderComponent* GetRenderComponent() const { return mPtrRenderComp; }

private:
	class RenderComponent* mPtrRenderComp;
};

