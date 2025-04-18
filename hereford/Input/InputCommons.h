#pragma once

using hInputSub = unsigned int;

enum class EInputAction
{
	RESERVED_DEFAULT = 0,

	PLAYER_MOVEMENT,
	FLY_MOVEMENT,
	TEST_INPUT,
};

enum class EInputS
{
	IDLE = 0,
	PRESSED,
	RELEASED,
	HOLD,
};