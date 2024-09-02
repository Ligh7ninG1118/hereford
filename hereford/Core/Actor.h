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

	// Update function called from Game (not overridable)
	void Update(float deltaTime);
	// ProcessInput function called from Game (not overridable)
	void ProcessInput(const std::vector<EInputState>& keyState, Uint32 mouseState, int mouseDeltaX, int mouseDeltaY);

	// Getters/setters
	Vector3 GetPosition() const { return mPosition; };
	void SetPosition(const Vector3& pos) { mPosition = pos; }
	Vector3 GetScale() const { return mScale; }
	void SetScale(const Vector3& scale) { mScale = scale; }
	Vector3 GetRotation() const { return mRotation; }
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

	// Returns component of type T, or null if doesn't exist
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
	// Any actor-specific update code (overridable)
	virtual void OnUpdate(float deltaTime);
	// Any actor-specific update code (overridable)
	virtual void OnProcessInput(const std::vector<EInputState>& keyState, Uint32 mouseState, int mouseDeltaX, int mouseDeltaY);

	class GameContext* mGame;
	// Actor's state
	ActorState mState;

	// Transform
	Vector3 mPosition;
	Vector3 mScale;
	Vector3 mRotation;

	// Components
	std::vector<class Component*> mComponents;

private:
	friend class Component;
	// Adds component to Actor (this is automatically called
	// in the component constructor)
	void AddComponent(class Component* c);
};

