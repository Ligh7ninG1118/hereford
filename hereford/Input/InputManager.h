#pragma once
#include "Math/Math.h"
#include "Input/InputCommons.h"
#include <unordered_map>
#include <vector>
#include <variant>
#include <functional>
#include <SDL2/SDL.h>

struct SwizzledInput
{
	SDL_Scancode mScancode;
	std::variant<bool,
		float,
		Vec2,
		Vec3> mModifier;
};

struct InputSub
{
	hInputSub mSubHandle;
	EInputAction mInputAction;
	EInputS mListenedState;
	std::function<void(EInputS)> mCallback;
};


class InputManager
{
public:
	InputManager();
	~InputManager();
	bool Initialize();
	void Shutdown();
	void Poll();

	// Subscribe to IDLE state means invoke callback for all states (Pressed, Released, Hold)
	[[nodiscard]] hInputSub Subscribe(EInputAction IA, std::function<void(EInputS)> callback, EInputS listenedState = EInputS::IDLE);
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

		const Uint8* rawKeyState = SDL_GetKeyboardState(nullptr);

		for (const auto& input : mIASwizzledMap[IA])
		{
			int keyState = rawKeyState[input.mScancode];
			if (std::holds_alternative<T>(input.mModifier))
			{
				T modifier = std::get<T>(input.mModifier);
				output += modifier * keyState;
			}
			else
			{
				// Log error message, function call type and input type not compatible
			}
		}

		return output;
	}


private:
	void AddKeyMappingToInputAction(EInputAction IA, SDL_Scancode keyCode);
	void CollectKeyStates();

	std::unordered_map<SDL_Scancode, EInputS> mKeyStateMap;

	std::unordered_map<SDL_Scancode, std::vector<EInputAction>> mInputMapping;
	std::unordered_map<EInputAction, std::vector<InputSub>> mIASubscriberMap;
	std::unordered_map<EInputAction, std::vector<SwizzledInput>> mIASwizzledMap;

	static hInputSub mInputSubCount;
};
