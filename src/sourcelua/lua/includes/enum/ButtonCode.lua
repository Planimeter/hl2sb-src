--========== Copyleft © 2010, Team Sandbox, Some rights reserved. ===========--
--
-- Purpose:
--
--===========================================================================--

include( "InputEnums.lua" )

-------------------------------------------------------------------------------
-- Button enum. "Buttons" are binary-state input devices (mouse buttons, keyboard keys)
-------------------------------------------------------------------------------
JOYSTICK_MAX_BUTTON_COUNT = 32
JOYSTICK_POV_BUTTON_COUNT = 4
JOYSTICK_AXIS_BUTTON_COUNT = MAX_JOYSTICK_AXES * 2

function JOYSTICK_BUTTON_INTERNAL( _joystick, _button ) return ( JOYSTICK_FIRST_BUTTON + ((_joystick) * JOYSTICK_MAX_BUTTON_COUNT) + (_button) ) end
function JOYSTICK_POV_BUTTON_INTERNAL( _joystick, _button ) return ( JOYSTICK_FIRST_POV_BUTTON + ((_joystick) * JOYSTICK_POV_BUTTON_COUNT) + (_button) ) end
function JOYSTICK_AXIS_BUTTON_INTERNAL( _joystick, _button ) return ( JOYSTICK_FIRST_AXIS_BUTTON + ((_joystick) * JOYSTICK_AXIS_BUTTON_COUNT) + (_button) ) end

function JOYSTICK_BUTTON( _joystick, _button ) return ( JOYSTICK_BUTTON_INTERNAL( _joystick, _button ) ) end
function JOYSTICK_POV_BUTTON( _joystick, _button ) return ( JOYSTICK_POV_BUTTON_INTERNAL( _joystick, _button ) ) end
function JOYSTICK_AXIS_BUTTON( _joystick, _button ) return ( JOYSTICK_AXIS_BUTTON_INTERNAL( _joystick, _button ) ) end

BUTTON_CODE_INVALID = -1
BUTTON_CODE_NONE = 0

KEY_FIRST = 0

KEY_NONE = KEY_FIRST
KEY_0 = 1
KEY_1 = 2
KEY_2 = 3
KEY_3 = 4
KEY_4 = 5
KEY_5 = 6
KEY_6 = 7
KEY_7 = 8
KEY_8 = 9
KEY_9 = 10
KEY_A = 11
KEY_B = 12
KEY_C = 13
KEY_D = 14
KEY_E = 15
KEY_F = 16
KEY_G = 17
KEY_H = 18
KEY_I = 19
KEY_J = 20
KEY_K = 21
KEY_L = 22
KEY_M = 23
KEY_N = 24
KEY_O = 25
KEY_P = 26
KEY_Q = 27
KEY_R = 28
KEY_S = 29
KEY_T = 30
KEY_U = 31
KEY_V = 32
KEY_W = 33
KEY_X = 34
KEY_Y = 35
KEY_Z = 36
KEY_PAD_0 = 37
KEY_PAD_1 = 38
KEY_PAD_2 = 39
KEY_PAD_3 = 40
KEY_PAD_4 = 41
KEY_PAD_5 = 42
KEY_PAD_6 = 43
KEY_PAD_7 = 44
KEY_PAD_8 = 45
KEY_PAD_9 = 46
KEY_PAD_DIVIDE = 47
KEY_PAD_MULTIPLY = 48
KEY_PAD_MINUS = 49
KEY_PAD_PLUS = 50
KEY_PAD_ENTER = 51
KEY_PAD_DECIMAL = 52
KEY_LBRACKET = 53
KEY_RBRACKET = 54
KEY_SEMICOLON = 55
KEY_APOSTROPHE = 56
KEY_BACKQUOTE = 57
KEY_COMMA = 58
KEY_PERIOD = 59
KEY_SLASH = 60
KEY_BACKSLASH = 61
KEY_MINUS = 62
KEY_EQUAL = 63
KEY_ENTER = 64
KEY_SPACE = 65
KEY_BACKSPACE = 66
KEY_TAB = 67
KEY_CAPSLOCK = 68
KEY_NUMLOCK = 69
KEY_ESCAPE = 70
KEY_SCROLLLOCK = 71
KEY_INSERT = 72
KEY_DELETE = 73
KEY_HOME = 74
KEY_END = 75
KEY_PAGEUP = 76
KEY_PAGEDOWN = 77
KEY_BREAK = 78
KEY_LSHIFT = 79
KEY_RSHIFT = 80
KEY_LALT = 81
KEY_RALT = 82
KEY_LCONTROL = 83
KEY_RCONTROL = 84
KEY_LWIN = 85
KEY_RWIN = 86
KEY_APP = 87
KEY_UP = 88
KEY_LEFT = 89
KEY_DOWN = 90
KEY_RIGHT = 91
KEY_F1 = 92
KEY_F2 = 93
KEY_F3 = 94
KEY_F4 = 95
KEY_F5 = 96
KEY_F6 = 97
KEY_F7 = 98
KEY_F8 = 99
KEY_F9 = 100
KEY_F10 = 101
KEY_F11 = 102
KEY_F12 = 103
KEY_CAPSLOCKTOGGLE = 104
KEY_NUMLOCKTOGGLE = 105
KEY_SCROLLLOCKTOGGLE = 106

KEY_LAST = KEY_SCROLLLOCKTOGGLE
KEY_COUNT = KEY_LAST - KEY_FIRST + 1

-- Mouse
MOUSE_FIRST = KEY_LAST + 1

MOUSE_LEFT = MOUSE_FIRST
MOUSE_RIGHT = 108
MOUSE_MIDDLE = 109
MOUSE_4 = 110
MOUSE_5 = 111
MOUSE_WHEEL_UP = 112	-- A fake button which is 'pressed' and 'released' when the wheel is moved up
MOUSE_WHEEL_DOWN = 113	-- A fake button which is 'pressed' and 'released' when the wheel is moved down

MOUSE_LAST = MOUSE_WHEEL_DOWN
MOUSE_COUNT = MOUSE_LAST - MOUSE_FIRST + 1

-- Joystick
JOYSTICK_FIRST = MOUSE_LAST + 1

JOYSTICK_FIRST_BUTTON = JOYSTICK_FIRST
JOYSTICK_LAST_BUTTON = JOYSTICK_BUTTON_INTERNAL( MAX_JOYSTICKS-1, JOYSTICK_MAX_BUTTON_COUNT-1 )
JOYSTICK_FIRST_POV_BUTTON = 146
JOYSTICK_LAST_POV_BUTTON = JOYSTICK_POV_BUTTON_INTERNAL( MAX_JOYSTICKS-1, JOYSTICK_POV_BUTTON_COUNT-1 )
JOYSTICK_FIRST_AXIS_BUTTON = 150
JOYSTICK_LAST_AXIS_BUTTON = JOYSTICK_AXIS_BUTTON_INTERNAL( MAX_JOYSTICKS-1, JOYSTICK_AXIS_BUTTON_COUNT-1 )

JOYSTICK_LAST = JOYSTICK_LAST_AXIS_BUTTON

BUTTON_CODE_LAST = 1
BUTTON_CODE_COUNT = BUTTON_CODE_LAST - KEY_FIRST + 1

-- Helpers for XBox 360
KEY_XBUTTON_UP = JOYSTICK_FIRST_POV_BUTTON	-- POV buttons
KEY_XBUTTON_RIGHT = 147
KEY_XBUTTON_DOWN = 148
KEY_XBUTTON_LEFT = 149

KEY_XBUTTON_A = JOYSTICK_FIRST_BUTTON		-- Buttons
KEY_XBUTTON_B = 115
KEY_XBUTTON_X = 116
KEY_XBUTTON_Y = 117
KEY_XBUTTON_LEFT_SHOULDER = 118
KEY_XBUTTON_RIGHT_SHOULDER = 119
KEY_XBUTTON_BACK = 120
KEY_XBUTTON_START = 121
KEY_XBUTTON_STICK1 = 122
KEY_XBUTTON_STICK2 = 123

KEY_XSTICK1_RIGHT = JOYSTICK_FIRST_AXIS_BUTTON	-- XAXIS POSITIVE
KEY_XSTICK1_LEFT = 151						-- XAXIS NEGATIVE
KEY_XSTICK1_DOWN = 152						-- YAXIS POSITIVE
KEY_XSTICK1_UP = 153						-- YAXIS NEGATIVE
KEY_XBUTTON_LTRIGGER = 154					-- ZAXIS POSITIVE
KEY_XBUTTON_RTRIGGER = 155					-- ZAXIS NEGATIVE
KEY_XSTICK2_RIGHT = 156						-- UAXIS POSITIVE
KEY_XSTICK2_LEFT = 157						-- UAXIS NEGATIVE
KEY_XSTICK2_DOWN = 158						-- VAXIS POSITIVE
KEY_XSTICK2_UP = 159						-- VAXIS NEGATIVE

function IsAlpha( code )
	return ( code >= KEY_A ) and ( code <= KEY_Z );
end

function IsAlphaNumeric( code )
	return ( code >= KEY_0 ) and ( code <= KEY_Z );
end

function IsSpace( code )
	return ( code == KEY_ENTER ) or ( code == KEY_TAB ) or ( code == KEY_SPACE );
end

function IsKeypad( code )
	return ( code >= MOUSE_FIRST ) and ( code <= KEY_PAD_DECIMAL );
end

function IsPunctuation( code )
	return ( code >= KEY_0 ) and ( code <= KEY_SPACE ) and not IsAlphaNumeric( code ) and not IsSpace( code ) and not IsKeypad( code );
end

function IsKeyCode( code )
	return ( code >= KEY_FIRST ) and ( code <= KEY_LAST );
end

function IsMouseCode( code )
	return ( code >= MOUSE_FIRST ) and ( code <= MOUSE_LAST );
end

function IsJoystickCode( code )
	return ( code >= JOYSTICK_FIRST ) and ( code <= JOYSTICK_LAST );
end

function IsJoystickButtonCode( code )
	return ( code >= JOYSTICK_FIRST_BUTTON ) and ( code <= JOYSTICK_LAST_BUTTON );
end

function IsJoystickPOVCode( code )
	return ( code >= JOYSTICK_FIRST_POV_BUTTON ) and ( code <= JOYSTICK_LAST_POV_BUTTON );
end

function IsJoystickAxisCode( code )
	return ( code >= JOYSTICK_FIRST_AXIS_BUTTON ) and ( code <= JOYSTICK_LAST_AXIS_BUTTON );
end
