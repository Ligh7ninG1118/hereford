
#include "Core/Actor.h"
#include "Graphics/Renderer.h"
#include "Graphics/LightComponent.h"

LightComponent::LightComponent(Actor* owner, Renderer& rendererRef)
	:
	Component(owner),
	mRenderer(rendererRef)
{
	mRenderer.AddLightComponent(this);
	Initialize();
}

LightComponent::~LightComponent()
{
	mRenderer.RemoveLightComponent(this);
}

bool LightComponent::Initialize()
{
	mDirLight.direction = Vec3(0.3f, -0.7f, 0.5f);
	mDirLight.ambient = Vec3(0.1f, 0.1f, 0.1f);
	mDirLight.diffuse = Vec3(0.6f, 0.6f, 0.6f);
	mDirLight.specular = Vec3(1.0f, 1.0f, 1.0f);

	mPointLight.ambient = Vec3(0.1f, 0.1f, 0.1f);
	mPointLight.diffuse = Vec3(0.6f, 0.6f, 0.6f);
	mPointLight.specular = Vec3(1.0f, 1.0f, 1.0f);

	mPointLight.constant = 1.0f;
	mPointLight.linear = 0.007f;
	mPointLight.quadratic = 0.0002f;

	return true;
}

Vector3 LightComponent::GetPosition() const
{
	return mOwner->GetPosition();
}
