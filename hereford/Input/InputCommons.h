#pragma once

using hInputSub = unsigned int;

enum class EInputAction
{
	RESERVED_DEFAULT = 0,

	GAME_QUIT,
	MOUSE_MOVEMENT,
	PLAYER_MOVEMENT,
	FLY_MOVEMENT,
	PLAYER_JUMP,
	PLAYER_CROUCH,
	PLAYER_SPRINT,
	PLAYER_INTERACT,
	WEAPON_FIRE_PRIMARY,
	WEAPON_FIRE_SECONDARY,
	WEAPON_RELOAD,
	WEAPON_SWAP_UP,
	WEAPON_SWAP_DOWN,
};

enum class EInputState
{
	IDLE = 0,
	PRESSED,
	RELEASED,
	HOLD,
};

//TODO: Could use a better way to unite SDL_Keycode and this
enum class HF_MOUSECODE
{
	RESERVED_DEFAULT = 0,

	LMB,
	MMB,
	RMB
};