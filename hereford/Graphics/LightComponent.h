#pragma once

#include "Core/Component.h"
#include "Graphics/LightStructs.h"

class LightComponent : public Component
{
public:
	LightComponent(class Actor* owner, class Renderer& rendererRef);
	~LightComponent();

	bool Initialize();

	DirLight GetDirLight() const { return mDirLight; }
	PointLight GetPointLight() const { return mPointLight; }
	struct Vector3 GetPosition() const;


private:
	/*LightType lightType;
	union LightData
	{
		DirLight dirLight;
		PointLight pointLight;
		SpotLight spotLight;
	};*/
	class Renderer& mRenderer;

	DirLight mDirLight;
	PointLight mPointLight;
};

