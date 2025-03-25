#pragma once

#include "Core/OldComponent.h"
#include "Graphics/LightStructs.h"

class LightComponent : public OldComponent
{
public:
	LightComponent(class Actor* owner, class Renderer& rendererRef);
	~LightComponent();

	bool Initialize();

	DirLight GetDirLight() const { return m_DirLight; }
	PointLight GetPointLight() const { return m_PointLight; }
	struct Vector3 GetPosition() const;


private:
	/*LightType lightType;
	union LightData
	{
		DirLight dirLight;
		PointLight pointLight;
		SpotLight spotLight;
	};*/
	class Renderer& m_Renderer;

	DirLight m_DirLight;
	PointLight m_PointLight;
};

