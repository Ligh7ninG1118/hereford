#include "UICrosshair.h"
#include "Asset/Shader.h"
#include "Graphics/Renderer.h"
#include "Gameplay/WeaponComponent.h"
#include <glad/glad.h>

UICrosshair::UICrosshair(Renderer* inPtrRenderer, Shader* inPtrShader)
	: UIElement(inPtrRenderer, inPtrShader),
	mCrosshairColor(Vec3(1.0f)),
	mOutlineColor(Vec3(0.25f)),
	mCrosshairTransparency(0.9f),
	mPtrWeaponComp(nullptr)
{
}

UICrosshair::~UICrosshair()
{
	GameEvent::Unsubscribe(mWeaponAimingEvent);
	delete mWeaponAimingEvent;
}

void UICrosshair::Initialize(WeaponComponent* inPtrWeaponComp)
{
	mPtrWeaponComp = inPtrWeaponComp;

	if (mVAO != 0)
		glDeleteVertexArrays(1, &mVAO);
	if (mVBO != 0)
		glDeleteBuffers(1, &mVBO);

	float quadVertices[] = {
		-1.0f,  1.0f,    // Top-left
		-1.0f, -1.0f,    // Bottom-left
		 1.0f, -1.0f,    // Bottom-right

		-1.0f,  1.0f,    // Top-left
		 1.0f, -1.0f,    // Bottom-right
		 1.0f,  1.0f    // Top-right
	};

	glGenVertexArrays(1, &mVAO);
	glGenBuffers(1, &mVBO);
	glBindVertexArray(mVAO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	Vec2 screenDimension = mPtrRenderer->GetScreenDimension();
	mPtrShader->Use();
	mPtrShader->SetVec2("screenDimension", screenDimension);
	mPtrShader->SetVec2("crosshairSize", 16.0f, 16.0f);
	mPtrShader->SetFloat("crosshairThickness", 1.0f);
	mPtrShader->SetFloat("crosshairGap", 10.0f);
	mPtrShader->SetFloat("outlineThickness", 1.0f);

	mPtrShader->SetVec4("crosshairColor", mCrosshairColor.mX, mCrosshairColor.mY, mCrosshairColor.mZ, mCrosshairTransparency);
	mPtrShader->SetVec4("outlineColor", mOutlineColor.mX, mOutlineColor.mY, mOutlineColor.mZ, mCrosshairTransparency);

	mWeaponAimingEvent = GameEvent::Subscribe<EventOnWeaponAiming>(std::bind(&UICrosshair::UpdateCrosshairEventHandler, this, std::placeholders::_1));

	UpdateCrosshair(0.0f);
}

void UICrosshair::UpdateCrosshairEventHandler(EventOnWeaponAiming inEvent)
{
	UpdateCrosshair(inEvent.aimingProgress);
}

void UICrosshair::UpdateCrosshair(float progress)
{
	float lerpTransparency = (1.0f - progress) * mCrosshairTransparency;

	float accDev = mPtrWeaponComp->GetAccuracyDeviation() * lerpTransparency;
	float gap = accDev * mPtrRenderer->GetScreenDimension().mX / (0.9f * 2.0f);

	mPtrShader->Use();
	mPtrShader->SetFloat("crosshairGap", gap);

	mPtrShader->SetVec4("crosshairColor", mCrosshairColor.mX, mCrosshairColor.mY, mCrosshairColor.mZ, lerpTransparency);
	mPtrShader->SetVec4("outlineColor", mOutlineColor.mX, mOutlineColor.mY, mOutlineColor.mZ, lerpTransparency);
}
