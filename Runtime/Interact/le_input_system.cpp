#include "le_input_system.h"

LEInputSystem::LEInputSystem() {

}

LEInputSystem::~LEInputSystem() {

}

bool LEInputSystem::isKeyDown(int keyCode) {
	return m_keyCodes[keyCode] == LEKeyState::Downed;
}

bool LEInputSystem::isKeyPressed(int keyCode) {
	return m_keyCodes[keyCode] == LEKeyState::Pressed;
}

bool LEInputSystem::isKeyRelease(int keyCode) {
	return m_keyCodes[keyCode] == LEKeyState::Released;
}

bool LEInputSystem::isMouseDown(int mouseBtn) {
	return m_mouseBtns[mouseBtn] == LEKeyState::Downed;
}

bool LEInputSystem::isMousePressed(int mouseBtn) {
	return m_mouseBtns[mouseBtn] == LEKeyState::Pressed;
}

bool LEInputSystem::isMouseRelease(int mouseBtn) {
	return m_mouseBtns[mouseBtn] == LEKeyState::Released;
}

glm::vec2 LEInputSystem::getMousePos() {
	return this->m_mouseState.mousePos;
}

glm::vec2 LEInputSystem::getMousePosDelta() {
	return this->m_mouseState.mousePosDelta;
}

void LEInputSystem::createContextForWindow(HWND hWnd) {
#if defined(__DINPUT_INCLUDED__)
	this->m_hInstance       = (HINSTANCE)::GetModuleHandleA(0);
	this->m_hWndMain        = hWnd;
	this->m_pDI             = NULL;
	this->m_pMouseDevice    = NULL;
	this->m_pKeyBoardDevice = NULL;

	LE_ASSERT_MSG(!FAILED(DirectInput8Create(this->m_hInstance,
		              DIRECTINPUT_VERSION,
		              IID_IDirectInput8,
		              reinterpret_cast<void**>(&this->m_pDI),
		              NULL)),"Create DirectInput context failed");

	// create keyboard device
	LE_ASSERT_MSG(!FAILED(this->m_pDI->CreateDevice(GUID_SysKeyboard, &this->m_pKeyBoardDevice, NULL)),
	              "Create DirectInput keyboard device failed");
	LE_ASSERT_MSG(!FAILED(this->m_pKeyBoardDevice->SetDataFormat(&c_dfDIKeyboard)),
	              "DirectInput operation 'set keyboard deivce data format' failed");
	LE_ASSERT_MSG(!FAILED(this->m_pKeyBoardDevice->SetCooperativeLevel(this->m_hWndMain, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)),
	              "DirectInput operation 'set keyboard deivce cooperative level' failed");
	m_pKeyBoardDevice->Acquire();

	// create mouse devic
	LE_ASSERT_MSG(!FAILED(this->m_pDI->CreateDevice(GUID_SysMouse, &this->m_pMouseDevice, NULL)),
	              "Create DirectInput mouse device failed");
	LE_ASSERT_MSG(!FAILED(this->m_pMouseDevice->SetDataFormat(&c_dfDIMouse)),
	              "DirectInput operation 'set mouse deivce data format' failed");
	LE_ASSERT_MSG(!FAILED(this->m_pMouseDevice->SetCooperativeLevel(this->m_hWndMain, DISCL_FOREGROUND | DISCL_EXCLUSIVE)),
	              "DirectInput operation 'set mouse deivce cooperative level' failed");
	m_pMouseDevice->Acquire();

	HANDLE hEvent = NULL;
	hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	LE_ASSERT_MSG((hEvent = CreateEvent(NULL, FALSE, FALSE, NULL)) != NULL,
	              "Create mouse event failed!");
	LE_ASSERT_MSG(!FAILED(this->m_pMouseDevice->SetEventNotification(hEvent)),
	              "DirectInput operation 'set mouse deivce event notification' failed");

	// create buffer for mouse
	DIPROPDWORD dipdw       = {0};
	dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipdw.diph.dwObj        = 0;
	dipdw.diph.dwHow        = DIPH_DEVICE;
	dipdw.dwData            = 16;
	LE_ASSERT_MSG(!FAILED(this->m_pMouseDevice->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph)),
	              "DirectInput operation 'set mouse deivce buffer size' failed");
	this->m_pMouseDevice->Acquire();
#endif // __DINPUT_INCLUDED__
}

void LEInputSystem::releaseContext() {
	if (this->m_pKeyBoardDevice != nullptr) {
		this->m_pKeyBoardDevice->Unacquire();
		this->m_pKeyBoardDevice->Release();
		this->m_pKeyBoardDevice = nullptr;
	}
	if (this->m_pMouseDevice != nullptr) {
		this->m_pMouseDevice->Unacquire();
		this->m_pMouseDevice->Release();
		this->m_pMouseDevice = nullptr;
	}
	if (this->m_pDI != nullptr) {
		this->m_pDI->Release();
		this->m_pDI = nullptr;
	}
}

void LEInputSystem::update() {
	// update keyboard
	uint32_t byteCount = sizeof(char) * 256;
	::memcpy(this->m_keyBoardState.previousState, 
	         this->m_keyBoardState.currentState, byteCount);
	HRESULT result;
	if (FAILED(result = m_pKeyBoardDevice->GetDeviceState(byteCount, this->m_keyBoardState.currentState))) {
		LE_ASSERT_MSG((result == DIERR_NOTACQUIRED) || (result == DIERR_INPUTLOST),
		              "fatal error occurred on the keyboard device");

		result = this->m_pKeyBoardDevice->Acquire();
		// wait for re-acquiring device
		while (result == DIERR_INPUTLOST) {
			result = this->m_pKeyBoardDevice->Acquire();
		}
		// If it is used by other applications, acquire it in the next Frame
		if (result == DIERR_OTHERAPPHASPRIO) {
			return;
		}
		if (SUCCEEDED(result)) {
			LE_ASSERT_MSG(!FAILED(this->m_pKeyBoardDevice->GetDeviceState(byteCount, this->m_keyBoardState.currentState)),
			              "fatal error occurred on the keyboard device");
		}
	}
	// update mouse
	DIDEVICEOBJECTDATA objectData[8] = { 0 };
	DWORD              dwNumElem     = 8;
	byteCount                        = 8 * sizeof(char);

	if (FAILED(result = this->m_pMouseDevice->GetDeviceData(byteCount, (DIDEVICEOBJECTDATA*)objectData, &dwNumElem, 0))) {
		LE_ASSERT_MSG((result == DIERR_NOTACQUIRED) || (result == DIERR_INPUTLOST),
		              "fatal error occurred on the mouse device");

		result = m_pMouseDevice->Acquire();
		// wait for re-acquiring device
		while (result == DIERR_INPUTLOST) {
			result = m_pMouseDevice->Acquire();
		}
		// If it is used by other applications, acquire it in the next Frame
		if (result == DIERR_OTHERAPPHASPRIO) {
			return;
		}
		if (SUCCEEDED(result)) {
			LE_ASSERT_MSG(!FAILED(this->m_pMouseDevice->GetDeviceData(byteCount, (DIDEVICEOBJECTDATA*)objectData, &dwNumElem, 0)),
			              "fatal error occurred on the mouse device");
		}
	}
	::memset(this->m_mouseState.isPressed, 0, sizeof(char) * 8);
	for (int i = 0; i < 8; i++) {
		switch (objectData[i].dwOfs) {
		case DIMOFS_X: {
			this->m_mouseState.mousePos.x += objectData[i].dwData;
			this->m_mouseState.mousePosDelta.x += objectData[i].dwData;
		}
		break;
		case DIMOFS_Y: {
			this->m_mouseState.mousePos.y += objectData[i].dwData;
			this->m_mouseState.mousePosDelta.y += objectData[i].dwData;
			break;
		case DIMOFS_Z: {
			this->m_mouseState.mouseScrollDelta += objectData[i].dwData;
		}
		break;
		default: {
			const int mouseBtnIndex = objectData[i].dwOfs - FIELD_OFFSET(DIMOUSESTATE2, rgbButtons);
			this->m_mouseState.isPressed[mouseBtnIndex] = objectData[i].dwData & 0x80;
		}
		break;
		}
		}
	}
}
