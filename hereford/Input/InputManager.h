#pragma once
#include <unordered_map>
#include <vector>
#include <variant>
#include <SDL2/SDL.h>

enum class EInputAction
{
	RESERVED_DEFAULT = 0,

	MOVEMENT,
};

struct SwizzledInput
{
	SDL_Scancode mScancode;
	std::variant<bool,
		float,
		Vec2> mModifier;
};

class InputManager
{
public:
	InputManager();
	~InputManager();
	bool Initialize();
	void Shutdown();
	void Update();

	template <typename T>
	T ReadValue(EInputAction inputAction)
	{

	}

private:
	std::unordered_map<SDL_Scancode, EInputAction> mInputMapping;
	std::unordered_map<EInputAction, std::vector<SwizzledInput>> mListenerMap;

};
