#pragma once

using hInputSub = unsigned int;

enum class EInputAction
{
	RESERVED_DEFAULT = 0,

	MOUSE_MOVEMENT,
	PLAYER_MOVEMENT,
	FLY_MOVEMENT,
	TEST_INPUT,
};

enum class EInputState
{
	IDLE = 0,
	PRESSED,
	RELEASED,
	HOLD,
};