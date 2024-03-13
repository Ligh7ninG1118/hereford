#pragma once

#include "../Core/Component.h"
#include "../Math/Math.h"



class CameraComponent : public Component
{
public:
	CameraComponent(class Actor* owner);
	~CameraComponent();

	void Update(float deltaTime) override;
	void ProcessInput(const Uint8* keyState, Uint32 mouseState, int mouseDeltaX, int mouseDeltaY) override;

	Mat4 GetViewMatrix() const;
	Mat4 GetPerspMatrix() const;
	Vec3 GetFrontVector() const
	{
		return Vec3(cos(DEG2RAD * m_Rotation.mY) * cos(DEG2RAD * m_Rotation.mX),
			sin(DEG2RAD * m_Rotation.mX),
			sin(DEG2RAD * m_Rotation.mY) * cos(DEG2RAD * m_Rotation.mX)).normalized();
	}
	Vec3 GetRightVector() const
	{
		return GetFrontVector().Cross(Vec3(0.0f, 1.0f, 0.0f)).normalized();
	}

	float GetMouseSens() const { return mouseSens; }
	Vec3 GetPositionOffset() const{ return m_PositionOffset; }

private:

	Vec3 m_PositionOffset;
	Vec3 m_Rotation;
	float m_ScreenRatio;
	float m_HorFOV;
	float m_NearPlane;
	float m_FarPlane;
	float mouseSens = 0.15f;
};