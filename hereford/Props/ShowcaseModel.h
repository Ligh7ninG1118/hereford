#pragma once
#include "Core/Actor.h"
#include "Util/Reflection.h"
#include <memory>

class ShowcaseModel : public Actor
{
	REFLECT(ShowcaseModel)

public:
	ShowcaseModel(class GameContext* gameCtx);
	~ShowcaseModel();

	class RenderComponent* GetRenderComponent() const { return mPtrRenderComp.get(); }

private:
	std::unique_ptr<class RenderComponent> mPtrRenderComp;
};

