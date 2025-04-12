#pragma once
#include "Math/Math.h"
#include <unordered_map>
#include <vector>
#include <variant>
#include <SDL2/SDL.h>

enum class EInputAction
{
	RESERVED_DEFAULT = 0,

	PLAYER_MOVEMENT,
	FLY_MOVEMENT,
};

struct SwizzledInput
{
	SDL_Scancode mScancode;
	std::variant<bool,
		float,
		Vec2,
		Vec3> mModifier;
};

class InputManager
{
public:
	InputManager();
	~InputManager();
	bool Initialize();
	void Shutdown();
	void Poll();

	template <typename T>
	T ReadValue(EInputAction inputAction)
	{
		auto itr = mListenerMap.find(inputAction);
		if (itr == mListenerMap.end())
		{
			//Log error message;
			return T{};
		}

		T output{};

		const Uint8* rawKeyState = SDL_GetKeyboardState(nullptr);

		for (const auto& input : mListenerMap[inputAction])
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
	std::unordered_map<SDL_Scancode, std::vector<EInputAction>> mInputMapping;
	std::unordered_map<EInputAction, std::vector<SwizzledInput>> mListenerMap;

};
