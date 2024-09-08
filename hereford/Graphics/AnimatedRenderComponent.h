#pragma once
#include "RenderComponent.h"
#include <memory>

class AnimatedRenderComponent : public RenderComponent
{
public:
	AnimatedRenderComponent(class Actor* owner, class Renderer& rendererRef);
	~AnimatedRenderComponent();

	inline std::weak_ptr<class Model> GetModel() const { return mPtrModel; }
	inline void SetModel(std::shared_ptr<class Model> inModel) { mPtrModel = inModel; }

private:
	std::shared_ptr<class Model> mPtrModel;
};

