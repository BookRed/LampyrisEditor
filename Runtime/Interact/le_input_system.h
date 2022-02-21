#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Interact
 * \File:    le_input_system.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_INPUT_SYSTEM_H
#define LE_INPUT_SYSTEM_H

// LE Includes
#include "../Base/le_assert.h"
#include "../Base/le_platform.h"
#include "../Base/le_singleton.h"

// STL Includes
#include <unordered_map>

// GLM Includes
#include <boost/mpl/size.hpp>
#include <glm/glm.hpp>

#if defined(LE_PLATFORM_WINDOWS)
  #include <DirectInput/dinput.h>
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#endif // !LE_PLATFORM_WINDOWS

enum class LEKeyState {
	Released = 0, 
	Pressed  = 1,
	Downed   = 2
};

enum LEMouseButton {
	Mouse_Left      = 0,
    Mouse_Right     = 1
};

struct KeyBoardState {
	char  previousState[256];			
	char  currentState[256];
};

struct MouseState {
	bool	  isPressed[8];
	glm::vec2 mousePos;
	glm::vec2 mousePosDelta;
	float     mouseScrollDelta;
};

enum LEKey {
	KEY_Escape     = 0x01000000,     
	KEY_Tab        = 0x01000001,
	KEY_Backtab    = 0x01000002,
	KEY_Backspace  = 0x01000003,
	KEY_Return     = 0x01000004,
	KEY_Enter      = 0x01000005,
	KEY_Insert     = 0x01000006,
	KEY_Delete     = 0x01000007,
	KEY_Pause      = 0x01000008,
	KEY_Print      = 0x01000009,             
	KEY_SysReq     = 0x0100000a,
	KEY_Clear      = 0x0100000b,
	KEY_Home       = 0x01000010,             
	KEY_End        = 0x01000011,
	KEY_Left       = 0x01000012,
	KEY_Up         = 0x01000013,
	KEY_Right      = 0x01000014,
	KEY_Down       = 0x01000015,
	KEY_PageUp     = 0x01000016,
	KEY_PageDown   = 0x01000017,
	KEY_Shift      = 0x01000020,             
	KEY_Control    = 0x01000021,
	KEY_Meta       = 0x01000022,
	KEY_Alt        = 0x01000023,
	KEY_CapsLock   = 0x01000024,
	KEY_NumLock    = 0x01000025,
	KEY_ScrollLock = 0x01000026,
	KEY_F1         = 0x01000030,             
	KEY_F2         = 0x01000031,
	KEY_F3         = 0x01000032,
	KEY_F4         = 0x01000033,
	KEY_F5         = 0x01000034,
	KEY_F6         = 0x01000035,
	KEY_F7         = 0x01000036,
	KEY_F8         = 0x01000037,
	KEY_F9         = 0x01000038,
	KEY_F10        = 0x01000039,
	KEY_F11        = 0x0100003a,
	KEY_F12        = 0x0100003b,
	KEY_0          = 0x00000030,
	KEY_1          = 0x00000031,
	KEY_2          = 0x00000032,
	KEY_3          = 0x00000033,
	KEY_4          = 0x00000034,
	KEY_5          = 0x00000035,
	KEY_6          = 0x00000036,
	KEY_7          = 0x00000037,
	KEY_8          = 0x00000038,
	KEY_9          = 0x00000039,
	KEY_A          = 0x00000041,
	KEY_B          = 0x00000042,
	KEY_C          = 0x00000043,
	KEY_D          = 0x00000044,
	KEY_E          = 0x00000045,
	KEY_F          = 0x00000046,
	KEY_G          = 0x00000047,
	KEY_H          = 0x00000048,
	KEY_I          = 0x00000049,
	KEY_J          = 0x0000004a,
	KEY_K          = 0x0000004b,
	KEY_L          = 0x0000004c,
	KEY_M          = 0x0000004d,
	KEY_N          = 0x0000004e,
	KEY_O          = 0x0000004f,
	KEY_P          = 0x00000050,
	KEY_Q          = 0x00000051,
	KEY_R          = 0x00000052,
	KEY_S          = 0x00000053,
	KEY_T          = 0x00000054,
	KEY_U          = 0x00000055,
	KEY_V          = 0x00000056,
	KEY_W          = 0x00000057,
	KEY_X          = 0x00000058,
	KEY_Y          = 0x00000059,
	KEY_Z          = 0x0000005a
};

class LEInputSystem:public Singleton<LEInputSystem> {
private:
	std::unordered_map<int, LEKeyState> m_keyCodes = {
	// Escape
	{ 0x01000000,LEKeyState::Released},
	// Tab
	{ 0x01000001,LEKeyState::Released},
	// BackTab
	{ 0x01000002,LEKeyState::Released},
	// BackSpace
	{ 0x01000003,LEKeyState::Released},
	// Return
	{ 0x01000004,LEKeyState::Released},
	// Enter
	{ 0x01000005,LEKeyState::Released},
	// Insert
	{ 0x01000006,LEKeyState::Released},
	// Delete
	{ 0x01000007,LEKeyState::Released},
	// Pause
	{ 0x01000008,LEKeyState::Released},
	// Print
	{ 0x01000009,LEKeyState::Released},
	// SysReq
	{ 0x0100000a,LEKeyState::Released},
	// Clear
	{ 0x0100000b,LEKeyState::Released},
	// Home
	{ 0x01000010,LEKeyState::Released},         
	// End
	{ 0x01000011,LEKeyState::Released},
	// Left
	{ 0x01000012,LEKeyState::Released},
	// Up
	{ 0x01000013,LEKeyState::Released},
	// Right
	{ 0x01000014,LEKeyState::Released},
	// Down
	{ 0x01000015,LEKeyState::Released},
	// Page Up
	{ 0x01000016,LEKeyState::Released},
	// Page Down
	{ 0x01000017,LEKeyState::Released},
	// Shift
	{ 0x01000020,LEKeyState::Released},
	// Ctrl
	{ 0x01000021,LEKeyState::Released},
	// Meta
	{ 0x01000022,LEKeyState::Released},
	// Alt
	{ 0x01000023,LEKeyState::Released},
	// Caps Lock
	{ 0x01000024,LEKeyState::Released},
	// Number Lock
	{ 0x01000025,LEKeyState::Released},
	// Scroll Lock
	{ 0x01000026,LEKeyState::Released},

	// From '0' to '9'
	{ 0x00000030,LEKeyState::Released},   // 0
	{ 0x00000031,LEKeyState::Released},	 // 1
	{ 0x00000032,LEKeyState::Released},	 // 2
	{ 0x00000033,LEKeyState::Released},	 // 3
	{ 0x00000034,LEKeyState::Released},	 // 4
	{ 0x00000035,LEKeyState::Released},	 // 5
	{ 0x00000036,LEKeyState::Released},	 // 6
	{ 0x00000037,LEKeyState::Released},	 // 7
	{ 0x00000038,LEKeyState::Released},	 // 8
	{ 0x00000039,LEKeyState::Released},	 // 9

	// From 'F1' to 'F12'
	{ 0x01000030,LEKeyState::Released},	 // F1
	{ 0x01000031,LEKeyState::Released},	 // F2
	{ 0x01000032,LEKeyState::Released},	 // F3
	{ 0x01000033,LEKeyState::Released},	 // F4
	{ 0x01000034,LEKeyState::Released},	 // F5
	{ 0x01000035,LEKeyState::Released},	 // F6
	{ 0x01000036,LEKeyState::Released},	 // F7
	{ 0x01000037,LEKeyState::Released},	 // F8
	{ 0x01000038,LEKeyState::Released},	 // F9
	{ 0x01000039,LEKeyState::Released},	 // F10
	{ 0x0100003a,LEKeyState::Released},	 // F11
	{ 0x0100003b,LEKeyState::Released},	 // F12

	// From 'A' to 'Z'
	{ 0x41,LEKeyState::Released}, // A
	{ 0x42,LEKeyState::Released}, // B
	{ 0x43,LEKeyState::Released}, // C
	{ 0x44,LEKeyState::Released}, // D
	{ 0x45,LEKeyState::Released}, // E
	{ 0x46,LEKeyState::Released}, // F
	{ 0x47,LEKeyState::Released}, // G
	{ 0x48,LEKeyState::Released}, // H
	{ 0x49,LEKeyState::Released}, // I
	{ 0x4a,LEKeyState::Released}, // J
	{ 0x4b,LEKeyState::Released}, // K
	{ 0x4c,LEKeyState::Released}, // L
	{ 0x4d,LEKeyState::Released}, // M
	{ 0x4e,LEKeyState::Released}, // N
	{ 0x4f,LEKeyState::Released}, // O
	{ 0x50,LEKeyState::Released}, // P
	{ 0x51,LEKeyState::Released}, // Q
	{ 0x52,LEKeyState::Released}, // R
	{ 0x53,LEKeyState::Released}, // S
	{ 0x54,LEKeyState::Released}, // T
	{ 0x55,LEKeyState::Released}, // U
	{ 0x56,LEKeyState::Released}, // V
	{ 0x57,LEKeyState::Released}, // W
	{ 0x58,LEKeyState::Released}, // X
	{ 0x59,LEKeyState::Released}, // Y
	{ 0x5a,LEKeyState::Released}  // Z
	};

	std::unordered_map<int, LEKeyState>  m_mouseBtns = {
	{ 0,LEKeyState::Released},  // Left
	{ 1,LEKeyState::Released}   // Left
	};

	KeyBoardState           m_keyBoardState;
	MouseState              m_mouseState;

	#if defined(__DINPUT_INCLUDED__)
	LPDIRECTINPUT8			m_pDI;
	LPDIRECTINPUTDEVICE8	m_pMouseDevice;
	LPDIRECTINPUTDEVICE8	m_pKeyBoardDevice;
	#endif // !__DINPUT_INCLUDED__

	#if defined(LE_PLATFORM_WINDOWS)
	HWND				    m_hWndMain;     
	HINSTANCE			    m_hInstance;
	#endif // !LE_PLATFORM_WINDOWS
public:
	          LEInputSystem();

	virtual  ~LEInputSystem();

	bool      isKeyDown(int keyCode);
		      
	bool      isKeyPressed(int keyCode);
		      
	bool      isKeyRelease(int keyCode);

	bool      isMouseDown(int mouseBtn);

	bool      isMousePressed(int mouseBtn);

	bool      isMouseRelease(int mouseBtn);

	glm::vec2 getMousePos();

	glm::vec2 getMousePosDelta();

	void      createContextForWindow(HWND hWnd);

	void      releaseContext();

	void      update();
	friend class LERenderTargetWindow;
};
#endif // !LE_INPUT_SYSTEM_H 