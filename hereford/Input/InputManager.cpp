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
	AddMappingToInputAction(EInputAction::GAME_QUIT, HF_InputCode::KEYBOARD_ESCAPE);
	AddMappingToInputAction(EInputAction::GAME_QUIT, HF_InputCode::CONTROLLER_BUTTON_START);

	mIASwizzledMap[EInputAction::PLAYER_MOVEMENT].push_back(SwizzledInput{ HF_InputCode::KEYBOARD_W, Vec2(1.0f, 0.0f) });
	mIASwizzledMap[EInputAction::PLAYER_MOVEMENT].push_back(SwizzledInput{ HF_InputCode::KEYBOARD_S, Vec2(-1.0f, 0.0f) });
	mIASwizzledMap[EInputAction::PLAYER_MOVEMENT].push_back(SwizzledInput{ HF_InputCode::KEYBOARD_D, Vec2(0.0f, 1.0f) });
	mIASwizzledMap[EInputAction::PLAYER_MOVEMENT].push_back(SwizzledInput{ HF_InputCode::KEYBOARD_A, Vec2(0.0f, -1.0f) });
	mIASwizzledMap[EInputAction::PLAYER_MOVEMENT].push_back(SwizzledInput{ HF_InputCode::CONTROLLER_AXIS_LEFTY, Vec2(-1.0f, 0.0f) });
	mIASwizzledMap[EInputAction::PLAYER_MOVEMENT].push_back(SwizzledInput{ HF_InputCode::CONTROLLER_AXIS_LEFTX, Vec2(0.0f, 1.0f) });

	mIASwizzledMap[EInputAction::PLAYER_VIEW].push_back(SwizzledInput{ HF_InputCode::MOUSE_DELTA_X, Vec2(1.0f, 0.0f) });
	mIASwizzledMap[EInputAction::PLAYER_VIEW].push_back(SwizzledInput{ HF_InputCode::MOUSE_DELTA_Y, Vec2(0.0f, 1.0f) });
	//TODO: Ramp up
	mIASwizzledMap[EInputAction::PLAYER_VIEW].push_back(SwizzledInput{ HF_InputCode::CONTROLLER_AXIS_RIGHTX, Vec2(10.0f, 0.0f) });
	mIASwizzledMap[EInputAction::PLAYER_VIEW].push_back(SwizzledInput{ HF_InputCode::CONTROLLER_AXIS_RIGHTY, Vec2(0.0f, -10.0f) });

	mIASwizzledMap[EInputAction::FLY_MOVEMENT].push_back(SwizzledInput{ HF_InputCode::KEYBOARD_W, Vec3(1.0f, 0.0f, 0.0f) });
	mIASwizzledMap[EInputAction::FLY_MOVEMENT].push_back(SwizzledInput{ HF_InputCode::KEYBOARD_S, Vec3(-1.0f, 0.0f, 0.0f) });
	mIASwizzledMap[EInputAction::FLY_MOVEMENT].push_back(SwizzledInput{ HF_InputCode::KEYBOARD_D, Vec3(0.0f, 0.0f, 1.0f) });
	mIASwizzledMap[EInputAction::FLY_MOVEMENT].push_back(SwizzledInput{ HF_InputCode::KEYBOARD_A, Vec3(0.0f, 0.0f, -1.0f)});
	mIASwizzledMap[EInputAction::FLY_MOVEMENT].push_back(SwizzledInput{ HF_InputCode::KEYBOARD_Q, Vec3(0.0f, -1.0f, 0.0f) });
	mIASwizzledMap[EInputAction::FLY_MOVEMENT].push_back(SwizzledInput{ HF_InputCode::KEYBOARD_E, Vec3(0.0f, 1.0f, 0.0f) });

	AddMappingToInputAction(EInputAction::PLAYER_JUMP, HF_InputCode::KEYBOARD_SPACE);
	AddMappingToInputAction(EInputAction::PLAYER_JUMP, HF_InputCode::CONTROLLER_BUTTON_A);
	AddMappingToInputAction(EInputAction::PLAYER_CROUCH, HF_InputCode::KEYBOARD_C);
	AddMappingToInputAction(EInputAction::PLAYER_CROUCH, HF_InputCode::CONTROLLER_BUTTON_B);
	AddMappingToInputAction(EInputAction::PLAYER_SPRINT, HF_InputCode::KEYBOARD_LSHIFT);
	AddMappingToInputAction(EInputAction::PLAYER_INTERACT, HF_InputCode::KEYBOARD_F);

	AddMappingToInputAction(EInputAction::WEAPON_FIRE_PRIMARY, HF_InputCode::MOUSE_BUTTON_LEFT);
	AddMappingToInputAction(EInputAction::WEAPON_FIRE_SECONDARY, HF_InputCode::MOUSE_BUTTON_RIGHT);
	AddMappingToInputAction(EInputAction::WEAPON_RELOAD, HF_InputCode::KEYBOARD_R);
	AddMappingToInputAction(EInputAction::WEAPON_SWAP_UP, HF_InputCode::KEYBOARD_1);
	AddMappingToInputAction(EInputAction::WEAPON_SWAP_DOWN, HF_InputCode::KEYBOARD_2);
	AddMappingToInputAction(EInputAction::WEAPON_SWAP_UP, HF_InputCode::MOUSE_SCROLL_UP);
	AddMappingToInputAction(EInputAction::WEAPON_SWAP_DOWN, HF_InputCode::MOUSE_SCROLL_DOWN);

	mRawKeyStates = SDL_GetKeyboardState(nullptr);

	// Find existing controller
	mController = FindController();

	return true;
}

void InputManager::Shutdown()
{
}

void InputManager::Poll()
{
	// Updating mouse states before other inputs
	UpdateMouseStates();
	UpdateInputStates();
	InvokeInputEvents();
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

void InputManager::UpdateInputStates()
{
	for (const auto& [inputCode, inputActionList] : mInputMapping)
	{
		bool rawState = static_cast<bool>(GetStateFromInputCode(inputCode));
		//TODO: Trigger hold state after set time
		if (mInputStateMap[inputCode] == EInputState::HOLD || mInputStateMap[inputCode] == EInputState::PRESSED)
			mInputStateMap[inputCode] = rawState ? EInputState::HOLD : EInputState::RELEASED;
		else
			mInputStateMap[inputCode] = rawState ? EInputState::PRESSED : EInputState::IDLE;
	}

}

void InputManager::InvokeInputEvents()
{
	for (const auto& [inputCode, inputState] : mInputStateMap)
	{
		if (inputState == EInputState::IDLE)
			continue;

		for (const auto& inputAction : mInputMapping[inputCode])
		{
			for (const auto& subscriber : mIASubscriberMap[inputAction])
			{
				if (subscriber.mListenedState == inputState || subscriber.mListenedState == EInputState::IDLE)
					subscriber.mCallback(inputState);
			}
		}
		
	}
}

void InputManager::UpdateMouseStates()
{
	int mouseDeltaX = 0;
	int mouseDeltaY = 0;
	mRawMouseState = SDL_GetRelativeMouseState(&mouseDeltaX, &mouseDeltaY);
	mMouseDelta.mX = static_cast<float>(mouseDeltaX);
	mMouseDelta.mY = static_cast<float>(-mouseDeltaY);

	// Update scroll wheel state
	if (mMouseScroll > 0)
	{
		mInputStateMap[HF_InputCode::MOUSE_SCROLL_UP] = EInputState::PRESSED;
		mInputStateMap[HF_InputCode::MOUSE_SCROLL_DOWN] = EInputState::IDLE;
	}
	else if (mMouseScroll < 0)
	{
		mInputStateMap[HF_InputCode::MOUSE_SCROLL_DOWN] = EInputState::PRESSED;
		mInputStateMap[HF_InputCode::MOUSE_SCROLL_UP] = EInputState::IDLE;
	}
	else
	{
		mInputStateMap[HF_InputCode::MOUSE_SCROLL_DOWN] = EInputState::IDLE;
		mInputStateMap[HF_InputCode::MOUSE_SCROLL_UP] = EInputState::IDLE;
	}
	// Consume input
	mMouseScroll = 0;
}

float InputManager::GetStateFromInputCode(HF_InputCode inputCode) const
{
	if (inputCode < HF_InputCode::KEYBOARD_GUARD_START)
	{
		// Handle warning message
	}
	// Keyboard
	else if (inputCode < HF_InputCode::MOUSE_GUARD_START)
	{
		return mRawKeyStates[static_cast<SDL_Scancode>(inputCode)];
	}
	// Mouse buttons
	else if (inputCode < HF_InputCode::CONTROLLER_BUTTON_GUARD_START)
	{
		// Already handled, pass the value along
		if (inputCode == HF_InputCode::MOUSE_SCROLL_UP || inputCode == HF_InputCode::MOUSE_SCROLL_DOWN)
			return (mInputStateMap.at(inputCode) == EInputState::PRESSED) ? 1 : 0;
		if (inputCode == HF_InputCode::MOUSE_DELTA_X)
			return mMouseDelta.mX;
		if (inputCode == HF_InputCode::MOUSE_DELTA_Y)
			return mMouseDelta.mY;

		int translatedMouseMask = SDL_BUTTON((static_cast<int>(inputCode) - static_cast<int>(HF_InputCode::MOUSE_GUARD_START)));
		return (mRawMouseState & translatedMouseMask);
	}
	// Controller buttons
	else if (inputCode < HF_InputCode::CONTROLLER_AXIS_GUARD_START)
	{
		SDL_GameControllerButton translatedCode = static_cast<SDL_GameControllerButton>((static_cast<int>(inputCode) - static_cast<int>(HF_InputCode::CONTROLLER_BUTTON_GUARD_START) - 1));
		if (mController == nullptr)
			return 0;
		return SDL_GameControllerGetButton(mController, translatedCode);
	}
	// Controller axes
	else
	{
		SDL_GameControllerAxis translatedAxis = static_cast<SDL_GameControllerAxis>((static_cast<int>(inputCode) - static_cast<int>(HF_InputCode::CONTROLLER_AXIS_GUARD_START) - 1));
		if (mController == nullptr)
			return 0;
		Sint16 value = SDL_GameControllerGetAxis(mController, translatedAxis);

		// Deadzone
		if (abs(value >> 8) < CONTROLLER_AXIS_DEADZONE)
			return 0;

		return ((float)(value >> 8)/INT8_MAX);
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

void InputManager::AddMappingToInputAction(EInputAction IA, HF_InputCode inputCode)
{
	mInputMapping[inputCode].push_back(IA);
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

