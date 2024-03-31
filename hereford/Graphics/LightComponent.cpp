
#include "Core/Actor.h"
#include "Graphics/Renderer.h"
#include "Graphics/LightComponent.h"

LightComponent::LightComponent(Actor* owner, Renderer& rendererRef)
	:
	Component(owner),
	m_Renderer(rendererRef)
{
	m_Renderer.AddLightComponent(this);
	Initialize();
}

LightComponent::~LightComponent()
{
	m_Renderer.RemoveLightComponent(this);
}

bool LightComponent::Initialize()
{
	m_DirLight.direction = Vec3(0.3f, -0.7f, 0.5f);
	m_DirLight.ambient = Vec3(0.1f, 0.1f, 0.1f);
	m_DirLight.diffuse = Vec3(0.6f, 0.6f, 0.6f);
	m_DirLight.specular = Vec3(1.0f, 1.0f, 1.0f);

	m_PointLight.ambient = Vec3(0.1f, 0.1f, 0.1f);
	m_PointLight.diffuse = Vec3(0.6f, 0.6f, 0.6f);
	m_PointLight.specular = Vec3(1.0f, 1.0f, 1.0f);

	m_PointLight.constant = 1.0f;
	m_PointLight.linear = 0.007f;
	m_PointLight.quadratic = 0.0002f;

	return true;
}

Vector3 LightComponent::GetPosition() const
{
	return mOwner->GetPosition();
}
