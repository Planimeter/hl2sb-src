--===== Copyright © 1996-2005, Valve Corporation, All rights reserved. ======--
--
-- Purpose:
--
--===========================================================================--

-- Standard maximum +/- value of a joystick axis
MAX_BUTTONSAMPLE			= 32768

if ( _X360 ) then
INVALID_USER_ID		= -1
else
INVALID_USER_ID		= XBX_INVALID_USER_ID
end

-------------------------------------------------------------------------------
-- Forward declarations:
-------------------------------------------------------------------------------

MAX_JOYSTICKS = 1
MOUSE_BUTTON_COUNT = 5

JOY_AXIS_X = 0
JOY_AXIS_Y = 1
JOY_AXIS_Z = 2
JOY_AXIS_R = 3
JOY_AXIS_U = 4
JOY_AXIS_V = 5
MAX_JOYSTICK_AXES = 6



-------------------------------------------------------------------------------
-- Extra mouse codes
-------------------------------------------------------------------------------
MS_WM_XBUTTONDOWN	= 0x020B
MS_WM_XBUTTONUP		= 0x020C
MS_WM_XBUTTONDBLCLK	= 0x020D
MS_MK_BUTTON4		= 0x0020
MS_MK_BUTTON5		= 0x0040

-------------------------------------------------------------------------------
-- Events
-------------------------------------------------------------------------------
IE_ButtonPressed = 0		-- m_nData contains a ButtonCode_t
IE_ButtonReleased = 1		-- m_nData contains a ButtonCode_t
IE_ButtonDoubleClicked = 2	-- m_nData contains a ButtonCode_t
IE_AnalogValueChanged = 3	-- m_nData contains an AnalogCode_t, m_nData2 contains the value

IE_FirstSystemEvent = 100
IE_Quit = IE_FirstSystemEvent
IE_ControllerInserted = 101		-- m_nData contains the controller ID
IE_ControllerUnplugged = 102	-- m_nData contains the controller ID

IE_FirstVguiEvent = 1000	-- Assign ranges for other systems that post user events here
IE_FirstAppEvent = 2000
