#pragma once
#include <Windows.h>
#include <WinUser.h>
#include <bitset>

class InputHandler
{
private:
	std::bitset<256> myCurrentKeyStates;
	std::bitset<256> myPreviousKeyStates;

	POINT myCurrentMousePos = {};
	POINT myLastMousePos = {};

	bool myCursorLock = false;
	bool myCursorVisible = true;

	bool myIsEnabled = true;

public:
	bool UpdateEvents(const HWND& aHWnd, const UINT& aMessage, const WPARAM& aWParam, const LPARAM& aLParam);

	//Keyboard and Mouse buttons. Use MK for mouse buttons e.g. MK_LBUTTON
	bool IsKeyPressed(const char& aKeyCode) const;
	bool IsKeyUp(const char& aKeyCode) const;
	bool IsKeyHeldDown(const char& aKeyCode) const;
	bool IsAnyKeyDown() const;

	//Mouse
	void SetCursorVisibility(const bool& aIsVisible);
	void SetCursorLock(const bool& aIsLocked);
	POINT GetMouseVelocity() const;

	void Refresh();
	void SetEnabled(const bool aIsEnabled);
};

