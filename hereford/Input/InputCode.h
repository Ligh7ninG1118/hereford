#pragma once

enum class HF_InputCode
{
	RESERVED_DEFAULT = 0,

	KEYBOARD_GUARD_START = 1,

    // Copied from SDL_Scancode
    KEYBOARD_A = 4,
    KEYBOARD_B = 5,
    KEYBOARD_C = 6,
    KEYBOARD_D = 7,
    KEYBOARD_E = 8,
    KEYBOARD_F = 9,
    KEYBOARD_G = 10,
    KEYBOARD_H = 11,
    KEYBOARD_I = 12,
    KEYBOARD_J = 13,
    KEYBOARD_K = 14,
    KEYBOARD_L = 15,
    KEYBOARD_M = 16,
    KEYBOARD_N = 17,
    KEYBOARD_O = 18,
    KEYBOARD_P = 19,
    KEYBOARD_Q = 20,
    KEYBOARD_R = 21,
    KEYBOARD_S = 22,
    KEYBOARD_T = 23,
    KEYBOARD_U = 24,
    KEYBOARD_V = 25,
    KEYBOARD_W = 26,
    KEYBOARD_X = 27,
    KEYBOARD_Y = 28,
    KEYBOARD_Z = 29,

    KEYBOARD_1 = 30,
    KEYBOARD_2 = 31,
    KEYBOARD_3 = 32,
    KEYBOARD_4 = 33,
    KEYBOARD_5 = 34,
    KEYBOARD_6 = 35,
    KEYBOARD_7 = 36,
    KEYBOARD_8 = 37,
    KEYBOARD_9 = 38,
    KEYBOARD_0 = 39,

    KEYBOARD_RETURN = 40,
    KEYBOARD_ESCAPE = 41,
    KEYBOARD_BACKSPACE = 42,
    KEYBOARD_TAB = 43,
    KEYBOARD_SPACE = 44,

    KEYBOARD_MINUS = 45,
    KEYBOARD_EQUALS = 46,
    KEYBOARD_LEFTBRACKET = 47,
    KEYBOARD_RIGHTBRACKET = 48,
    KEYBOARD_BACKSLASH = 49, /**< Located at the lower left of the return
                                  *   key on ISO keyboards and at the right end
                                  *   of the QWERTY row on ANSI keyboards.
                                  *   Produces REVERSE SOLIDUS (backslash) and
                                  *   VERTICAL LINE in a US layout, REVERSE
                                  *   SOLIDUS and VERTICAL LINE in a UK Mac
                                  *   layout, NUMBER SIGN and TILDE in a UK
                                  *   Windows layout, DOLLAR SIGN and POUND SIGN
                                  *   in a Swiss German layout, NUMBER SIGN and
                                  *   APOSTROPHE in a German layout, GRAVE
                                  *   ACCENT and POUND SIGN in a French Mac
                                  *   layout, and ASTERISK and MICRO SIGN in a
                                  *   French Windows layout.
                                  */
    KEYBOARD_NONUSHASH = 50, /**< ISO USB keyboards actually use this code
                                  *   instead of 49 for the same key, but all
                                  *   OSes I've seen treat the two codes
                                  *   identically. So, as an implementor, unless
                                  *   your keyboard generates both of those
                                  *   codes and your OS treats them differently,
                                  *   you should generate KEYBOARD_BACKSLASH
                                  *   instead of this code. As a user, you
                                  *   should not rely on this code because SDL
                                  *   will never generate it with most (all?)
                                  *   keyboards.
                                  */
    KEYBOARD_SEMICOLON = 51,
    KEYBOARD_APOSTROPHE = 52,
    KEYBOARD_GRAVE = 53, /**< Located in the top left corner (on both ANSI
                              *   and ISO keyboards). Produces GRAVE ACCENT and
                              *   TILDE in a US Windows layout and in US and UK
                              *   Mac layouts on ANSI keyboards, GRAVE ACCENT
                              *   and NOT SIGN in a UK Windows layout, SECTION
                              *   SIGN and PLUS-MINUS SIGN in US and UK Mac
                              *   layouts on ISO keyboards, SECTION SIGN and
                              *   DEGREE SIGN in a Swiss German layout (Mac:
                              *   only on ISO keyboards), CIRCUMFLEX ACCENT and
                              *   DEGREE SIGN in a German layout (Mac: only on
                              *   ISO keyboards), SUPERSCRIPT TWO and TILDE in a
                              *   French Windows layout, COMMERCIAL AT and
                              *   NUMBER SIGN in a French Mac layout on ISO
                              *   keyboards, and LESS-THAN SIGN and GREATER-THAN
                              *   SIGN in a Swiss German, German, or French Mac
                              *   layout on ANSI keyboards.
                              */
    KEYBOARD_COMMA = 54,
    KEYBOARD_PERIOD = 55,
    KEYBOARD_SLASH = 56,

    KEYBOARD_CAPSLOCK = 57,

    KEYBOARD_F1 = 58,
    KEYBOARD_F2 = 59,
    KEYBOARD_F3 = 60,
    KEYBOARD_F4 = 61,
    KEYBOARD_F5 = 62,
    KEYBOARD_F6 = 63,
    KEYBOARD_F7 = 64,
    KEYBOARD_F8 = 65,
    KEYBOARD_F9 = 66,
    KEYBOARD_F10 = 67,
    KEYBOARD_F11 = 68,
    KEYBOARD_F12 = 69,

    KEYBOARD_PRINTSCREEN = 70,
    KEYBOARD_SCROLLLOCK = 71,
    KEYBOARD_PAUSE = 72,
    KEYBOARD_INSERT = 73, /**< insert on PC, help on some Mac keyboards (but
                                   does send code 73, not 117) */
    KEYBOARD_HOME = 74,
    KEYBOARD_PAGEUP = 75,
    KEYBOARD_DELETE = 76,
    KEYBOARD_END = 77,
    KEYBOARD_PAGEDOWN = 78,
    KEYBOARD_RIGHT = 79,
    KEYBOARD_LEFT = 80,
    KEYBOARD_DOWN = 81,
    KEYBOARD_UP = 82,

    KEYBOARD_NUMLOCKCLEAR = 83, /**< num lock on PC, clear on Mac keyboards
                                     */
    KEYBOARD_KP_DIVIDE = 84,
    KEYBOARD_KP_MULTIPLY = 85,
    KEYBOARD_KP_MINUS = 86,
    KEYBOARD_KP_PLUS = 87,
    KEYBOARD_KP_ENTER = 88,
    KEYBOARD_KP_1 = 89,
    KEYBOARD_KP_2 = 90,
    KEYBOARD_KP_3 = 91,
    KEYBOARD_KP_4 = 92,
    KEYBOARD_KP_5 = 93,
    KEYBOARD_KP_6 = 94,
    KEYBOARD_KP_7 = 95,
    KEYBOARD_KP_8 = 96,
    KEYBOARD_KP_9 = 97,
    KEYBOARD_KP_0 = 98,
    KEYBOARD_KP_PERIOD = 99,

    KEYBOARD_NONUSBACKSLASH = 100, /**< This is the additional key that ISO
                                        *   keyboards have over ANSI ones,
                                        *   located between left shift and Y.
                                        *   Produces GRAVE ACCENT and TILDE in a
                                        *   US or UK Mac layout, REVERSE SOLIDUS
                                        *   (backslash) and VERTICAL LINE in a
                                        *   US or UK Windows layout, and
                                        *   LESS-THAN SIGN and GREATER-THAN SIGN
                                        *   in a Swiss German, German, or French
                                        *   layout. */
    KEYBOARD_APPLICATION = 101, /**< windows contextual menu, compose */
    KEYBOARD_POWER = 102, /**< The USB document says this is a status flag,
                               *   not a physical key - but some Mac keyboards
                               *   do have a power key. */
    KEYBOARD_KP_EQUALS = 103,
    KEYBOARD_F13 = 104,
    KEYBOARD_F14 = 105,
    KEYBOARD_F15 = 106,
    KEYBOARD_F16 = 107,
    KEYBOARD_F17 = 108,
    KEYBOARD_F18 = 109,
    KEYBOARD_F19 = 110,
    KEYBOARD_F20 = 111,
    KEYBOARD_F21 = 112,
    KEYBOARD_F22 = 113,
    KEYBOARD_F23 = 114,
    KEYBOARD_F24 = 115,
    KEYBOARD_EXECUTE = 116,
    KEYBOARD_HELP = 117,    /**< AL Integrated Help Center */
    KEYBOARD_MENU = 118,    /**< Menu (show menu) */
    KEYBOARD_SELECT = 119,
    KEYBOARD_STOP = 120,    /**< AC Stop */
    KEYBOARD_AGAIN = 121,   /**< AC Redo/Repeat */
    KEYBOARD_UNDO = 122,    /**< AC Undo */
    KEYBOARD_CUT = 123,     /**< AC Cut */
    KEYBOARD_COPY = 124,    /**< AC Copy */
    KEYBOARD_PASTE = 125,   /**< AC Paste */
    KEYBOARD_FIND = 126,    /**< AC Find */
    KEYBOARD_MUTE = 127,
    KEYBOARD_VOLUMEUP = 128,
    KEYBOARD_VOLUMEDOWN = 129,
    /* not sure whether there's a reason to enable these */
    /*     KEYBOARD_LOCKINGCAPSLOCK = 130,  */
    /*     KEYBOARD_LOCKINGNUMLOCK = 131, */
    /*     KEYBOARD_LOCKINGSCROLLLOCK = 132, */
KEYBOARD_KP_COMMA = 133,
KEYBOARD_KP_EQUALSAS400 = 134,

KEYBOARD_INTERNATIONAL1 = 135, /**< used on Asian keyboards, see
                                        footnotes in USB doc */
    KEYBOARD_INTERNATIONAL2 = 136,
    KEYBOARD_INTERNATIONAL3 = 137, /**< Yen */
    KEYBOARD_INTERNATIONAL4 = 138,
    KEYBOARD_INTERNATIONAL5 = 139,
    KEYBOARD_INTERNATIONAL6 = 140,
    KEYBOARD_INTERNATIONAL7 = 141,
    KEYBOARD_INTERNATIONAL8 = 142,
    KEYBOARD_INTERNATIONAL9 = 143,
    KEYBOARD_LANG1 = 144, /**< Hangul/English toggle */
    KEYBOARD_LANG2 = 145, /**< Hanja conversion */
    KEYBOARD_LANG3 = 146, /**< Katakana */
    KEYBOARD_LANG4 = 147, /**< Hiragana */
    KEYBOARD_LANG5 = 148, /**< Zenkaku/Hankaku */
    KEYBOARD_LANG6 = 149, /**< reserved */
    KEYBOARD_LANG7 = 150, /**< reserved */
    KEYBOARD_LANG8 = 151, /**< reserved */
    KEYBOARD_LANG9 = 152, /**< reserved */

    KEYBOARD_ALTERASE = 153,    /**< Erase-Eaze */
    KEYBOARD_SYSREQ = 154,
    KEYBOARD_CANCEL = 155,      /**< AC Cancel */
    KEYBOARD_CLEAR = 156,
    KEYBOARD_PRIOR = 157,
    KEYBOARD_RETURN2 = 158,
    KEYBOARD_SEPARATOR = 159,
    KEYBOARD_OUT = 160,
    KEYBOARD_OPER = 161,
    KEYBOARD_CLEARAGAIN = 162,
    KEYBOARD_CRSEL = 163,
    KEYBOARD_EXSEL = 164,

    KEYBOARD_KP_00 = 176,
    KEYBOARD_KP_000 = 177,
    KEYBOARD_THOUSANDSSEPARATOR = 178,
    KEYBOARD_DECIMALSEPARATOR = 179,
    KEYBOARD_CURRENCYUNIT = 180,
    KEYBOARD_CURRENCYSUBUNIT = 181,
    KEYBOARD_KP_LEFTPAREN = 182,
    KEYBOARD_KP_RIGHTPAREN = 183,
    KEYBOARD_KP_LEFTBRACE = 184,
    KEYBOARD_KP_RIGHTBRACE = 185,
    KEYBOARD_KP_TAB = 186,
    KEYBOARD_KP_BACKSPACE = 187,
    KEYBOARD_KP_A = 188,
    KEYBOARD_KP_B = 189,
    KEYBOARD_KP_C = 190,
    KEYBOARD_KP_D = 191,
    KEYBOARD_KP_E = 192,
    KEYBOARD_KP_F = 193,
    KEYBOARD_KP_XOR = 194,
    KEYBOARD_KP_POWER = 195,
    KEYBOARD_KP_PERCENT = 196,
    KEYBOARD_KP_LESS = 197,
    KEYBOARD_KP_GREATER = 198,
    KEYBOARD_KP_AMPERSAND = 199,
    KEYBOARD_KP_DBLAMPERSAND = 200,
    KEYBOARD_KP_VERTICALBAR = 201,
    KEYBOARD_KP_DBLVERTICALBAR = 202,
    KEYBOARD_KP_COLON = 203,
    KEYBOARD_KP_HASH = 204,
    KEYBOARD_KP_SPACE = 205,
    KEYBOARD_KP_AT = 206,
    KEYBOARD_KP_EXCLAM = 207,
    KEYBOARD_KP_MEMSTORE = 208,
    KEYBOARD_KP_MEMRECALL = 209,
    KEYBOARD_KP_MEMCLEAR = 210,
    KEYBOARD_KP_MEMADD = 211,
    KEYBOARD_KP_MEMSUBTRACT = 212,
    KEYBOARD_KP_MEMMULTIPLY = 213,
    KEYBOARD_KP_MEMDIVIDE = 214,
    KEYBOARD_KP_PLUSMINUS = 215,
    KEYBOARD_KP_CLEAR = 216,
    KEYBOARD_KP_CLEARENTRY = 217,
    KEYBOARD_KP_BINARY = 218,
    KEYBOARD_KP_OCTAL = 219,
    KEYBOARD_KP_DECIMAL = 220,
    KEYBOARD_KP_HEXADECIMAL = 221,

    KEYBOARD_LCTRL = 224,
    KEYBOARD_LSHIFT = 225,
    KEYBOARD_LALT = 226, /**< alt, option */
    KEYBOARD_LGUI = 227, /**< windows, command (apple), meta */
    KEYBOARD_RCTRL = 228,
    KEYBOARD_RSHIFT = 229,
    KEYBOARD_RALT = 230, /**< alt gr, option */
    KEYBOARD_RGUI = 231, /**< windows, command (apple), meta */

    MOUSE_GUARD_START,

    MOUSE_BUTTON_LEFT,
    MOUSE_BUTTON_MIDDLE,
    MOUSE_BUTTON_RIGHT,
    MOUSE_SCROLL_UP,
    MOUSE_SCROLL_DOWN,
    MOUSE_DELTA_X,
    MOUSE_DELTA_Y,

    CONTROLLER_BUTTON_GUARD_START,

    CONTROLLER_BUTTON_A,
    CONTROLLER_BUTTON_B,
    CONTROLLER_BUTTON_X,
    CONTROLLER_BUTTON_Y,
    CONTROLLER_BUTTON_BACK,
    CONTROLLER_BUTTON_GUIDE,
    CONTROLLER_BUTTON_START,
    CONTROLLER_BUTTON_LEFTSTICK,
    CONTROLLER_BUTTON_RIGHTSTICK,
    CONTROLLER_BUTTON_LEFTSHOULDER,
    CONTROLLER_BUTTON_RIGHTSHOULDER,
    CONTROLLER_BUTTON_DPAD_UP,
    CONTROLLER_BUTTON_DPAD_DOWN,
    CONTROLLER_BUTTON_DPAD_LEFT,
    CONTROLLER_BUTTON_DPAD_RIGHT,

    CONTROLLER_AXIS_GUARD_START,

    CONTROLLER_AXIS_LEFTX,
    CONTROLLER_AXIS_LEFTY,
    CONTROLLER_AXIS_RIGHTX,
    CONTROLLER_AXIS_RIGHTY,
    CONTROLLER_AXIS_TRIGGERLEFT,
    CONTROLLER_AXIS_TRIGGERRIGHT,
};