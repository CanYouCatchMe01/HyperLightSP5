#include "stdafx.h"
#include "InputHandler.h"

bool InputHandler::UpdateEvents(const HWND& aHWnd, const UINT& aMessage, const WPARAM& aWParam, const LPARAM& aLParam)
{
	aLParam;

	if (!myIsEnabled)
	{
		myCurrentKeyStates = 0;
		myPreviousKeyStates = 0;
		return false;
	}

	//Lock cursor to the center of the window
	if (myCursorLock)
	{
		RECT WindowRect;
		GetWindowRect(aHWnd, &WindowRect);
		const int centerX = (WindowRect.right + WindowRect.left) / 2;
		const int centerY = (WindowRect.top + WindowRect.bottom) / 2;
		SetCursorPos(centerX, centerY);
	}
	switch (aMessage)
	{
	//Down
	case WM_KEYDOWN:
		myCurrentKeyStates[aWParam] = true;
		break;
	case WM_LBUTTONDOWN:
		myCurrentKeyStates[MK_LBUTTON] = true;
		break;
	case WM_MBUTTONDOWN:
		myCurrentKeyStates[MK_MBUTTON] = true;
		break;
	case WM_RBUTTONDOWN:
		myCurrentKeyStates[MK_RBUTTON] = true;
		break;
	//Up
	case WM_KEYUP:
		myCurrentKeyStates[aWParam] = false;
		break;
	case WM_LBUTTONUP:
		myCurrentKeyStates[MK_LBUTTON] = false;
		break;
	case WM_MBUTTONUP:
		myCurrentKeyStates[MK_MBUTTON] = false;
		break;
	case WM_RBUTTONUP:;
		myCurrentKeyStates[MK_RBUTTON] = false;
		break;
	//Mouse move
	case WM_MOUSEMOVE:
		myLastMousePos = myCurrentMousePos;
		GetCursorPos(&myCurrentMousePos);
		break;
	default:
		return false; //Not a valid message found
	}
	return true;
}

bool InputHandler::IsKeyPressed(const char& aKeyCode) const
{
	//The key IS presses now, but it was NOT presses the previous time
	return myCurrentKeyStates[aKeyCode] && !myPreviousKeyStates[aKeyCode];
}

bool InputHandler::IsKeyUp(const char& aKeyCode) const
{
	//The key is NOT presses now, but was WAS presses the previous time
	return !myCurrentKeyStates[aKeyCode] && myPreviousKeyStates[aKeyCode];
}

bool InputHandler::IsKeyHeldDown(const char& aKeyCode) const
{
	return myCurrentKeyStates[aKeyCode];
}

bool InputHandler::IsAnyKeyDown() const
{
	return myCurrentKeyStates.any();
}

void InputHandler::SetCursorVisibility(const bool& aIsVisible)
{
	if (aIsVisible == myCursorVisible)
	{
		return;
	}

	ShowCursor(aIsVisible);
	myCursorVisible = aIsVisible;
}

void InputHandler::SetCursorLock(const bool& aIsLocked)
{
	myCursorLock = aIsLocked;
}

POINT InputHandler::GetMouseVelocity() const
{
	POINT velocityPoint;
	velocityPoint.x = myCurrentMousePos.x - myLastMousePos.x;
	velocityPoint.y = myCurrentMousePos.y - myLastMousePos.y;
	return velocityPoint;
}

void InputHandler::Refresh()
{
	myPreviousKeyStates = myCurrentKeyStates;
}

void InputHandler::SetEnabled(const bool aIsEnabled)
{
	myIsEnabled = aIsEnabled;
	if (!myIsEnabled)
	{
		myCurrentKeyStates = 0;
		myPreviousKeyStates = 0;
	}
}
