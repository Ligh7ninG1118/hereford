#pragma once
#include "Math/Math.h"
#include "Input/InputCommons.h"
#include "Input/InputCode.h"
#include <unordered_map>
#include <vector>
#include <variant>
#include <functional>
#include <SDL2/SDL.h>

struct SwizzledInput
{
	HF_InputCode mInputCode;
	std::variant<bool,
		float,
		Vec2,
		Vec3> mModifier;
};

struct InputSub
{
	hInputSub mSubHandle;
	EInputAction mInputAction;
	EInputState mListenedState;
	std::function<void(EInputState)> mCallback;
};


class InputManager
{
public:
	InputManager();
	~InputManager();
	bool Initialize();
	void Shutdown();
	void Poll();
	void HandleEvent(SDL_Event event);

	// Subscribe to IDLE state means invoke callback for all states (Pressed, Released, Hold)
	[[nodiscard]] hInputSub Subscribe(EInputAction IA, std::function<void(EInputState)> callback, EInputState listenedState = EInputState::IDLE);
	void Unsubscribe(EInputAction IA, hInputSub hSub);

	template <typename T>
	T ReadValue(EInputAction IA)
	{
		auto itr = mIASwizzledMap.find(IA);
		if (itr == mIASwizzledMap.end())
		{
			//Log error message;
			return T{};
		}

		T output{};

		for (const auto& input : mIASwizzledMap[IA])
		{
			float inputState = GetStateFromInputCode(input.mInputCode);
			if (std::holds_alternative<T>(input.mModifier))
			{
				T modifier = std::get<T>(input.mModifier);
				output += modifier * inputState;
			}
			else
			{
				// Log error message, function call type and input type not compatible
			}
		}

		return output;
	}

	Vec2 ReadMouseDelta() const;


private:
	void AddMappingToInputAction(EInputAction IA, HF_InputCode keyCode);

	void UpdateInputStates();
	void InvokeInputEvents();
	// Handling special cases: mouse delta, scroll wheel
	void UpdateMouseStates();

	float GetStateFromInputCode(HF_InputCode inputCode) const;

	SDL_GameController* FindController() const;

	std::unordered_map<HF_InputCode, EInputState> mInputStateMap;
	std::unordered_map<HF_InputCode, std::vector<EInputAction>> mInputMapping;

	std::unordered_map<EInputAction, std::vector<InputSub>> mIASubscriberMap;
	std::unordered_map<EInputAction, std::vector<SwizzledInput>> mIASwizzledMap;

	Vec2 mMouseDelta;
	int mMouseScroll;
	const Uint8* mRawKeyStates;
	Uint32 mRawMouseState;

	SDL_GameController* mController;

	static hInputSub mInputSubCount;

	const int8 CONTROLLER_AXIS_DEADZONE = 25;
};
