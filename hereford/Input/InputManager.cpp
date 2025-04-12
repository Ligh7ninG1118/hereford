#include "InputManager.h"

InputManager::InputManager()
{
}

InputManager::~InputManager()
{
}

bool InputManager::Initialize()
{
	mListenerMap[EInputAction::PLAYER_MOVEMENT].push_back(SwizzledInput{ SDL_SCANCODE_W, Vec2(1.0f, 0.0f) });
	mListenerMap[EInputAction::PLAYER_MOVEMENT].push_back(SwizzledInput{ SDL_SCANCODE_S, Vec2(-1.0f, 0.0f) });
	mListenerMap[EInputAction::PLAYER_MOVEMENT].push_back(SwizzledInput{ SDL_SCANCODE_D, Vec2(0.0f, 1.0f) });
	mListenerMap[EInputAction::PLAYER_MOVEMENT].push_back(SwizzledInput{ SDL_SCANCODE_A, Vec2(0.0f, -1.0f) });

	mListenerMap[EInputAction::FLY_MOVEMENT].push_back(SwizzledInput{ SDL_SCANCODE_W, Vec3(1.0f, 0.0f, 0.0f) });
	mListenerMap[EInputAction::FLY_MOVEMENT].push_back(SwizzledInput{ SDL_SCANCODE_S, Vec3(-1.0f, 0.0f, 0.0f) });
	mListenerMap[EInputAction::FLY_MOVEMENT].push_back(SwizzledInput{ SDL_SCANCODE_D, Vec3(0.0f, 0.0f, 1.0f) });
	mListenerMap[EInputAction::FLY_MOVEMENT].push_back(SwizzledInput{ SDL_SCANCODE_A, Vec3(0.0f, 0.0f, -1.0f)});
	mListenerMap[EInputAction::FLY_MOVEMENT].push_back(SwizzledInput{ SDL_SCANCODE_Q, Vec3(0.0f, -1.0f, 0.0f) });
	mListenerMap[EInputAction::FLY_MOVEMENT].push_back(SwizzledInput{ SDL_SCANCODE_E, Vec3(0.0f, 1.0f, 0.0f) });

	return true;
}

void InputManager::Shutdown()
{
}

void InputManager::Update()
{
}
