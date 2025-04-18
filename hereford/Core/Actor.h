#pragma once
#include <vector>
#include <string>
#include <SDL2/SDL_stdinc.h>
#include "Math/Math.h"
#include "Util/Enums.h"

enum class EActorState
{
	Enabled,
	Disabled,
	Destroy
};

class Actor
{
public:
	Actor(class GameContext* game);
	virtual ~Actor();
	virtual const std::string& GetClassName() const = 0;

	void Update(float deltaTime);

	Vector3 GetPosition() const { return mPosition; };
	Vector3 GetScale() const { return mScale; }
	Vector3 GetRotation() const { return mRotation; }
	
	void SetPosition(const Vector3& pos) { mPosition = pos; }
	void SetScale(const Vector3& scale) { mScale = scale; }
	void SetRotation(const Vector3& rotation) { mRotation = rotation; }
	
	
	Vector3 GetForward() const
	{
		return Vector3(cos(DEG2RAD * mRotation.mY)* cos(DEG2RAD * mRotation.mX),
			sin(DEG2RAD * mRotation.mX),
			sin(DEG2RAD * mRotation.mY)* cos(DEG2RAD * mRotation.mX)).normalized();
	}

	EActorState GetState() const { return mState; }
	void SetState(EActorState state, bool setComponents = false);

	class GameContext* GetGameContext() const { return mGameCtx; }

	template <typename T>
	T* GetComponent() const
	{
		for (auto c : mComponents)
		{
			T* t = dynamic_cast<T*>(c);
			if (t != nullptr)
			{
				return t;
			}
		}

		return nullptr;
	}

protected:
	virtual void OnUpdate(float deltaTime);

	class GameContext* mGameCtx;
	EActorState mState;

	// Transform
	Vector3 mPosition;
	Vector3 mScale;
	Vector3 mRotation;

	std::vector<class Component*> mComponents;

private:
	friend class Component;
	void AddComponent(class Component* c);
};

