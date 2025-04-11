#pragma once
#include <unordered_map>
#include <vector>
#include <SDL2/SDL.h>

enum class EInputAction
{
	RESERVED_DEFAULT = 0,

	MOVEMENT,
};

class InputManager
{
public:
	InputManager();
	~InputManager();
	bool Initialize();
	void Shutdown();
	void Update();

private:
	std::unordered_map<SDL_Scancode, EInputAction> mInputMapping;
	//std::unordered_map<EInputAction, std::vector<>> mListenerMap;

};

