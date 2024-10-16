#pragma once

enum class EInputState
{
	NOT_PRESSED = 0,
	KEY_HOLD,
	KEY_DOWN,
	KEY_UP
};

enum EMouseState
{
	LMB_NOT_PRESSED		= 1,
	LMB_HOLD			= 1 << 1,
	LMB_DOWN			= 1 << 2,
	LMB_UP				= 1 << 3,

	RMB_NOT_PRESSED		= 1 << 4,
	RMB_HOLD			= 1 << 5,
	RMB_DOWN			= 1 << 6,
	RMB_UP				= 1 << 7,

	MMB_NOT_PRESSED		= 1 << 8,
	MMB_HOLD			= 1 << 9,
	MMB_DOWN			= 1 << 10,
	MMB_UP				= 1 << 11,

	SCROLL_IDLE			= 1 << 12,
	SCROLL_UP			= 1 << 13,
	SCROLL_DOWN			= 1 << 14,
};

