#pragma once
#include "GamePad.h"
#include "InputHandler.h"

enum class InputAction
{
	DOWN,
	UP,
	LEFT,
	RIGHT,
	SHOOT,
	DASH,
	JUMP,
	CLIMB,
	RESET,
	SELECT,
	BACK,
	ESC,
	DEBUG1,
	DEBUG2,
	DEBUG3
};

class InputManager
{

public:

	InputManager(InputHandler& anInputHandler);
	~InputManager();

	void Update();
	//void Vibrate();
	bool IsPressed(InputAction anAction);
	bool IsReleased(InputAction anAction);
	bool IsHeld(InputAction anAction);
	bool IsScroll(InputAction aDirection);
	inline const bool ControllerConnected() const { return myGamePad->IsConnected(); }
	Tga2D::Vector2f MovementDir();

private:
	Gamepad* myGamePad = new Gamepad(1);
	InputHandler* myKeyboard = new InputHandler();

	//Timer myVibrateTimer;
	//Timer myActivatedTimer;

	bool IsXboxButtonDown(Gamepad::Button aButton);
	bool IsXboxButtonPressed(Gamepad::Button aButton);
	//bool IsXboxButtonReleased(Gamepad::Button aButton);
	bool myKeysPressed[14] = { false };
	bool myControllerConnected = false;
	bool myLTDown = false;
	bool myRTDown = false;

	float myScrollUpTimer = 0.0f;
	float myScrollDownTimer = 0.0f;
	float myScrollRightTimer = 0.0f;
	float myScrollLeftTimer = 0.0f;

	float myVerticalScrollCooldown = 0.2f;
	float myHorizontalScrollCooldown = 0.15f;
	float myVibrateDuration = 1.0f;
	float myControllerTextDuration = 4.f;


};