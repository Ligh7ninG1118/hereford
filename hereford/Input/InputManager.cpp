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
	//TODO: serialize into config
	// Add key mappings
	AddKeyMappingToInputAction(EInputAction::GAME_QUIT, SDL_SCANCODE_ESCAPE);

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

	AddKeyMappingToInputAction(EInputAction::PLAYER_JUMP, SDL_SCANCODE_SPACE);
	AddKeyMappingToInputAction(EInputAction::PLAYER_CROUCH, SDL_SCANCODE_C);
	AddKeyMappingToInputAction(EInputAction::PLAYER_SPRINT, SDL_SCANCODE_LSHIFT);
	AddKeyMappingToInputAction(EInputAction::PLAYER_INTERACT, SDL_SCANCODE_F);
	AddControllerMappingToInputAction(EInputAction::PLAYER_JUMP, SDL_CONTROLLER_BUTTON_A);
	AddControllerMappingToInputAction(EInputAction::PLAYER_CROUCH, SDL_CONTROLLER_BUTTON_B);

	AddMouseMappingToInputAction(EInputAction::WEAPON_FIRE_PRIMARY, HF_MOUSECODE::LMB);
	AddMouseMappingToInputAction(EInputAction::WEAPON_FIRE_SECONDARY, HF_MOUSECODE::RMB);
	AddKeyMappingToInputAction(EInputAction::WEAPON_RELOAD, SDL_SCANCODE_R);
	AddKeyMappingToInputAction(EInputAction::WEAPON_SWAP_UP, SDL_SCANCODE_1);
	AddKeyMappingToInputAction(EInputAction::WEAPON_SWAP_DOWN, SDL_SCANCODE_2);
	AddMouseMappingToInputAction(EInputAction::WEAPON_SWAP_UP, HF_MOUSECODE::SCROLL_UP);
	AddMouseMappingToInputAction(EInputAction::WEAPON_SWAP_DOWN, HF_MOUSECODE::SCROLL_DOWN);

	

	// Find existing controller
	mController = FindController();

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
	InvokeMouseEvents();
	UpdateControllerStates();
	InvokeControllerEvents();
}

void InputManager::HandleEvent(SDL_Event event)
{
	switch (event.type)
	{
	case SDL_CONTROLLERDEVICEADDED:
		if (mController == nullptr)
			mController = SDL_GameControllerOpen(event.cdevice.which);
		break;
	case SDL_CONTROLLERDEVICEREMOVED:
		if (mController != nullptr && event.cdevice.which == SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(mController)))
		{
			SDL_GameControllerClose(mController);
			mController = FindController();
		}
		break;
	case SDL_MOUSEWHEEL:
		mMouseScroll = event.wheel.y;
		break;
	default:
		break;
	}
}

void InputManager::UpdateKeyStates()
{
	const Uint8* rawKeyState = SDL_GetKeyboardState(nullptr);

	for (const auto& inputPair : mKeyInputMapping)
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
			for (const auto& inputAction : mKeyInputMapping[keyState.first])
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

	// Update LMB, MMB, RMB states
	for (int i = 1; i < 4; i++)
	{
		int mouseMask = SDL_BUTTON(i);
		HF_MOUSECODE mouseCode = static_cast<HF_MOUSECODE>(i);

		if (mMouseStateMap[mouseCode] == EInputState::HOLD || mMouseStateMap[mouseCode] == EInputState::PRESSED)
			mMouseStateMap[mouseCode] = (rawMouseState & mouseMask) ? EInputState::HOLD : EInputState::RELEASED;
		else
			mMouseStateMap[mouseCode] = (rawMouseState & mouseMask) ? EInputState::PRESSED : EInputState::IDLE;
	}

	// Update scroll wheel state
	if (mMouseScroll > 0)
	{
		mMouseStateMap[HF_MOUSECODE::SCROLL_UP] = EInputState::PRESSED;
		mMouseStateMap[HF_MOUSECODE::SCROLL_DOWN] = EInputState::IDLE;
	}
	else if (mMouseScroll < 0)
	{
		mMouseStateMap[HF_MOUSECODE::SCROLL_DOWN] = EInputState::PRESSED;
		mMouseStateMap[HF_MOUSECODE::SCROLL_UP] = EInputState::IDLE;
	}
	else
	{
		mMouseStateMap[HF_MOUSECODE::SCROLL_DOWN] = EInputState::IDLE;
		mMouseStateMap[HF_MOUSECODE::SCROLL_UP] = EInputState::IDLE;
	}
	// Consume input
	mMouseScroll = 0;
}

void InputManager::InvokeMouseEvents()
{
	for (const auto& mouseState : mMouseStateMap)
	{
		if (mouseState.second != EInputState::IDLE)
		{
			for (const auto& inputAction : mMouseInputMapping[mouseState.first])
			{
				for (const auto& subscriber : mIASubscriberMap[inputAction])
				{
					if (subscriber.mListenedState == mouseState.second || subscriber.mListenedState == EInputState::IDLE)
						subscriber.mCallback(mouseState.second);
				}
			}
		}
	}
}

void InputManager::UpdateControllerStates()
{
	for (const auto& inputPair : mControllerButtonInputMapping)
	{
		SDL_GameControllerButton buttonCode = inputPair.first;
		Uint8 buttonState = SDL_GameControllerGetButton(mController, inputPair.first);
		if (mControllerButtonStateMap[buttonCode] == EInputState::HOLD || mControllerButtonStateMap[buttonCode] == EInputState::PRESSED)
			mControllerButtonStateMap[buttonCode] = buttonState ? EInputState::HOLD : EInputState::RELEASED;
		else
			mControllerButtonStateMap[buttonCode] = buttonState ? EInputState::PRESSED : EInputState::IDLE;
	}
}

void InputManager::InvokeControllerEvents()
{
	for (const auto& buttonState : mControllerButtonStateMap)
	{
		if (buttonState.second != EInputState::IDLE)
		{
			for (const auto& inputAction : mControllerButtonInputMapping[buttonState.first])
			{
				for (const auto& subscriber : mIASubscriberMap[inputAction])
				{
					if (subscriber.mListenedState == buttonState.second || subscriber.mListenedState == EInputState::IDLE)
						subscriber.mCallback(buttonState.second);
				}
			}
		}
	}
}

SDL_GameController* InputManager::FindController() const
{
	int numJoysticks = SDL_NumJoysticks();
	for (int i = 0; i < numJoysticks; i++)
	{
		if (SDL_IsGameController(i))
		{
			return SDL_GameControllerOpen(i);
		}
	}
	return nullptr;
}

Vec2 InputManager::ReadMouseDelta() const
{
	return mMouseDelta;
}

void InputManager::AddKeyMappingToInputAction(EInputAction IA, SDL_Scancode keyCode)
{
	mKeyInputMapping[keyCode].push_back(IA);
}

void InputManager::AddMouseMappingToInputAction(EInputAction IA, HF_MOUSECODE mouseCode)
{
	mMouseInputMapping[mouseCode].push_back(IA);
}

void InputManager::AddControllerMappingToInputAction(EInputAction IA, SDL_GameControllerButton buttonCode)
{
	mControllerButtonInputMapping[buttonCode].push_back(IA);
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

