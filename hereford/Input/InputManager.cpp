#include "InputManager.h"

// Reserve 0 for error output
hInputSub InputManager::mInputSubCount = 1;	


InputManager::InputManager()
{
}

InputManager::~InputManager()
{
}

bool InputManager::Initialize()
{
	mIASwizzledMap[EInputAction::PLAYER_MOVEMENT].push_back(SwizzledInput{ SDL_SCANCODE_W, Vec2(1.0f, 0.0f) });
	mIASwizzledMap[EInputAction::PLAYER_MOVEMENT].push_back(SwizzledInput{ SDL_SCANCODE_S, Vec2(-1.0f, 0.0f) });
	mIASwizzledMap[EInputAction::PLAYER_MOVEMENT].push_back(SwizzledInput{ SDL_SCANCODE_D, Vec2(0.0f, 1.0f) });
	mIASwizzledMap[EInputAction::PLAYER_MOVEMENT].push_back(SwizzledInput{ SDL_SCANCODE_A, Vec2(0.0f, -1.0f) });

	mIASwizzledMap[EInputAction::FLY_MOVEMENT].push_back(SwizzledInput{ SDL_SCANCODE_W, Vec3(1.0f, 0.0f, 0.0f) });
	mIASwizzledMap[EInputAction::FLY_MOVEMENT].push_back(SwizzledInput{ SDL_SCANCODE_S, Vec3(-1.0f, 0.0f, 0.0f) });
	mIASwizzledMap[EInputAction::FLY_MOVEMENT].push_back(SwizzledInput{ SDL_SCANCODE_D, Vec3(0.0f, 0.0f, 1.0f) });
	mIASwizzledMap[EInputAction::FLY_MOVEMENT].push_back(SwizzledInput{ SDL_SCANCODE_A, Vec3(0.0f, 0.0f, -1.0f)});
	mIASwizzledMap[EInputAction::FLY_MOVEMENT].push_back(SwizzledInput{ SDL_SCANCODE_Q, Vec3(0.0f, -1.0f, 0.0f) });
	mIASwizzledMap[EInputAction::FLY_MOVEMENT].push_back(SwizzledInput{ SDL_SCANCODE_E, Vec3(0.0f, 1.0f, 0.0f) });

	AddKeyMappingToInputAction(EInputAction::TEST_INPUT, SDL_SCANCODE_F);

	return true;
}

void InputManager::Shutdown()
{
}

void InputManager::Poll()
{
	UpdateKeyStates();
	InvokeKeyEvents();
	UpdateMouseStates();
}

void InputManager::UpdateKeyStates()
{
	const Uint8* rawKeyState = SDL_GetKeyboardState(nullptr);

	for (const auto& inputPair : mInputMapping)
	{
		SDL_Scancode keyCode = inputPair.first;
		//TODO: Trigger hold state after set time
		if (mKeyStateMap[keyCode] == EInputState::HOLD || mKeyStateMap[keyCode] == EInputState::PRESSED)
			mKeyStateMap[keyCode] = rawKeyState[keyCode] ? EInputState::HOLD : EInputState::RELEASED;
		else
			mKeyStateMap[keyCode] = rawKeyState[keyCode] ? EInputState::PRESSED : EInputState::IDLE;
	}

}

void InputManager::InvokeKeyEvents()
{
	for (const auto& keyState : mKeyStateMap)
	{
		if (keyState.second != EInputState::IDLE)
		{
			for (const auto& inputAction : mInputMapping[keyState.first])
			{
				for (const auto& subscriber : mIASubscriberMap[inputAction])
				{
					if (subscriber.mListenedState == keyState.second || subscriber.mListenedState == EInputState::IDLE)
						subscriber.mCallback(keyState.second);
				}
			}
		}
	}
}

void InputManager::UpdateMouseStates()
{
	int mouseDeltaX = 0;
	int mouseDeltaY = 0;
	Uint32 rawMouseState = SDL_GetRelativeMouseState(&mouseDeltaX, &mouseDeltaY);
	mMouseDelta.mX = static_cast<float>(mouseDeltaX);
	mMouseDelta.mY = static_cast<float>(-mouseDeltaY);


}

Vec2 InputManager::ReadMouseDelta() const
{
	return mMouseDelta;
}

void InputManager::AddKeyMappingToInputAction(EInputAction IA, SDL_Scancode keyCode)
{
	mInputMapping[keyCode].push_back(IA);
}




hInputSub InputManager::Subscribe(EInputAction IA, std::function<void(EInputState)> callback, EInputState listenedState)
{
	InputSub newSub{};
	newSub.mSubHandle = mInputSubCount++;
	newSub.mInputAction = IA;
	newSub.mListenedState = listenedState;
	newSub.mCallback = callback;

	mIASubscriberMap[IA].push_back(newSub);

	return newSub.mSubHandle;
}

void InputManager::Unsubscribe(EInputAction IA, hInputSub hSub)
{
	auto itr = mIASubscriberMap.find(IA);
	if (itr == mIASubscriberMap.end())
	{
		//Log error message;
		return;
	}

	auto subList = itr->second;
	for (auto it = subList.begin();it != subList.end();)
	{
		if (it->mSubHandle == hSub)
		{
			subList.erase(it);
			break;
		}
		else
			it++;
	}
}

