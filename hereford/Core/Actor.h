#pragma once
#include <vector>
#include <SDL2/SDL_stdinc.h>
#include "Math/Math.h"
#include "Util/Enums.h"

enum class ActorState
{
	Active,
	Paused,
	Destroy
};

class Actor
{
public:
	Actor(class GameContext* game);

	virtual ~Actor();

	void Update(float deltaTime);
	void ProcessInput(const std::vector<EInputState>& keyState, Uint32 mouseState, int mouseDeltaX, int mouseDeltaY);

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

	ActorState GetState() const { return mState; }
	void SetState(ActorState state) { mState = state; }

	class GameContext* GetGame() { return mGame; }

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
	virtual void OnProcessInput(const std::vector<EInputState>& keyState, Uint32 mouseState, int mouseDeltaX, int mouseDeltaY);

	class GameContext* mGame;
	ActorState mState;

	// Transform
	Vector3 mPosition;
	Vector3 mScale;
	Vector3 mRotation;

	std::vector<class Component*> mComponents;

private:
	friend class Component;
	void AddComponent(class Component* c);
};

