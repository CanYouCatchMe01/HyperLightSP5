#include "stdafx.h"
#include "InputManager.h"



InputManager::InputManager(InputHandler& anInputHandler)
{
	myKeyboard = &anInputHandler;
}

InputManager::~InputManager()
{
	delete myGamePad;
	myGamePad = nullptr;
}

void InputManager::Update()
{
	myGamePad->Update();

	if (!myGamePad->LeftTrigger())
		myLTDown = false;
	if (!myGamePad->RightTrigger())
		myRTDown = false;
}


bool InputManager::IsXboxButtonDown(Gamepad::Button aButton)
{
	return myGamePad->IsButtonPressed(aButton);
}


bool InputManager::IsXboxButtonPressed(Gamepad::Button aButton)
{
	if (!myGamePad->IsConnected())
		return false;

	bool isKeyPressed = false;
	int buttonIndex = 0;

	switch (aButton)
	{
	case Gamepad::Button::DPAD_UP:
	{
		isKeyPressed = myGamePad->IsButtonPressed(Gamepad::Button::DPAD_UP);
		buttonIndex = 0;
		break;
	}

	case Gamepad::Button::DPAD_DOWN:
	{
		isKeyPressed = myGamePad->IsButtonPressed(Gamepad::Button::DPAD_DOWN);
		buttonIndex = 1;
		break;
	}

	case Gamepad::Button::DPAD_LEFT:
	{
		isKeyPressed = myGamePad->IsButtonPressed(Gamepad::Button::DPAD_LEFT);
		buttonIndex = 2;
		break;
	}

	case Gamepad::Button::DPAD_RIGHT:
	{
		isKeyPressed = myGamePad->IsButtonPressed(Gamepad::Button::DPAD_RIGHT);
		buttonIndex = 3;
		break;
	}
	case Gamepad::Button::START:
	{
		isKeyPressed = myGamePad->IsButtonPressed(Gamepad::Button::START);
		buttonIndex = 4;
		break;
	}
	case Gamepad::Button::BACK:
	{
		isKeyPressed = myGamePad->IsButtonPressed(Gamepad::Button::BACK);
		buttonIndex = 5;
		break;
	}
	case Gamepad::Button::LEFT_THUMB:
	{
		isKeyPressed = myGamePad->IsButtonPressed(Gamepad::Button::LEFT_THUMB);
		buttonIndex = 6;
		break;
	}
	case Gamepad::Button::RIGHT_THUMB:
	{
		isKeyPressed = myGamePad->IsButtonPressed(Gamepad::Button::RIGHT_THUMB);
		buttonIndex = 7;
		break;
	}
	case Gamepad::Button::LEFT_SHOULDER:
	{
		isKeyPressed = myGamePad->IsButtonPressed(Gamepad::Button::LEFT_SHOULDER);
		buttonIndex = 8;
		break;
	}
	case Gamepad::Button::RIGHT_SHOULDER:
	{
		isKeyPressed = myGamePad->IsButtonPressed(Gamepad::Button::RIGHT_SHOULDER);
		buttonIndex = 9;
		break;
	}
	case Gamepad::Button::A:
	{
		isKeyPressed = myGamePad->IsButtonPressed(Gamepad::Button::A);
		buttonIndex = 10;
		break;
	}

	case Gamepad::Button::B:
	{
		isKeyPressed = myGamePad->IsButtonPressed(Gamepad::Button::B);
		buttonIndex = 11;
		break;
	}

	case Gamepad::Button::X:
	{
		isKeyPressed = myGamePad->IsButtonPressed(Gamepad::Button::X);
		buttonIndex = 12;
		break;
	}

	case Gamepad::Button::Y:
	{
		isKeyPressed = myGamePad->IsButtonPressed(Gamepad::Button::Y);
		buttonIndex = 13;
		break;
	}

	default:
		break;
	}



	if (isKeyPressed && !myKeysPressed[buttonIndex])
	{
		myKeysPressed[buttonIndex] = true;
		return true;
	}

	else if (!isKeyPressed && myKeysPressed[buttonIndex])
	{
		myKeysPressed[buttonIndex] = false;
	}

	return false;
}

bool InputManager::IsPressed(InputAction anAction)
{
	bool isXboxControl = false;
	bool input = false;
	if (myGamePad->IsConnected())
		isXboxControl = true;

	switch (anAction)
	{
	case InputAction::DOWN:
	{
		if (isXboxControl)
		{
			input = IsXboxButtonPressed(Gamepad::Button::DPAD_DOWN);
		}
		if (!input)
			input = (myKeyboard->IsKeyPressed(VK_DOWN));
		if (!input)
			input = (myKeyboard->IsKeyPressed('S'));
		return input;
		break;
	}

	case InputAction::UP:
	{
		if (isXboxControl)
		{
			input = IsXboxButtonPressed(Gamepad::Button::DPAD_UP);
		}
		if (!input)
			input = (myKeyboard->IsKeyPressed(VK_UP));
		if (!input)
			input = (myKeyboard->IsKeyPressed('W'));
		return input;
		break;
	}
	case InputAction::LEFT:
	{
		if (isXboxControl)
		{
			input = IsXboxButtonPressed(Gamepad::Button::DPAD_LEFT);
		}
		if (!input)
			input = (myKeyboard->IsKeyPressed(VK_LEFT));
		if (!input)
			input = (myKeyboard->IsKeyPressed('A'));
		return input;
		break;
	}
	case InputAction::RIGHT:
	{
		if (isXboxControl)
		{
			input = IsXboxButtonPressed(Gamepad::Button::DPAD_RIGHT);
		}
		if (!input)
			input = (myKeyboard->IsKeyPressed(VK_RIGHT));
		if (!input)
			input = (myKeyboard->IsKeyPressed('D'));
		return input;
		break;
	}
	case InputAction::SHOOT:
	{
		if (isXboxControl && !myRTDown)
		{
			input = myGamePad->RightTrigger();
			if (input)
				myRTDown = true;
		}

		if (!input)
			input = myKeyboard->IsKeyPressed('Z');
		return input;

		break;
	}
	case InputAction::DASH:
	{
		if (isXboxControl)
		{
			input = IsXboxButtonPressed(Gamepad::Button::X);
		}

		if (!input)
			input = myKeyboard->IsKeyPressed('X');
		return input;

		break;
	}
	case InputAction::JUMP:
	{
		if (isXboxControl)
		{
			input = IsXboxButtonPressed(Gamepad::Button::A);
		}
		if (!input)
			input = myKeyboard->IsKeyPressed('C');
		if (input)
			//	std::cout << "";
			return input;

		break;
	}
	case InputAction::CLIMB:
	{
		if (isXboxControl && !myLTDown)
		{
			input = myGamePad->LeftTrigger();
			if (input)
				myLTDown = true;
		}

		if (!input)
			input = myKeyboard->IsKeyPressed('C');
		return input;

		break;
	}
	case InputAction::SELECT:
	{
		if (isXboxControl)
		{
			input = IsXboxButtonPressed(Gamepad::Button::A);
		}
		if (!input)
			input = myKeyboard->IsKeyPressed(VK_SPACE);
		if (!input)
			input = myKeyboard->IsKeyPressed(VK_RETURN);
		return input;

		break;
	}

	case InputAction::GODMODE:
		if (!input)
			input = myKeyboard->IsKeyPressed('Q');
		return input;
		break;

	case InputAction::RESET:
		if (isXboxControl)
		{
			input = IsXboxButtonPressed(Gamepad::Button::Y);
		}
		if (!input)
			input = myKeyboard->IsKeyPressed('Q');
		return input;

		break;
	case InputAction::BACK:
	{
		if (isXboxControl)
		{
			input = IsXboxButtonPressed(Gamepad::Button::B);
		}
		if (!input)
			input = myKeyboard->IsKeyPressed(VK_DELETE);
		return input;

		break;
	}
	case InputAction::ESC:
	{
		if (isXboxControl)
		{
			input = IsXboxButtonPressed(Gamepad::Button::START);
		}
		if (!input)
			input = myKeyboard->IsKeyPressed(VK_ESCAPE);
		return input;

		break;
	}
	case InputAction::DEBUG1:
	{
		return false;
		//if (isXboxControl)
		//{
		//	input = IsXboxButtonPressed(Gamepad::Button::LEFT_SHOULDER);
		//}
		//if (!input)
		//	input = myKeyboard->IsKeyDown(VK_F1);
		//return input;

		//break;
	}
	case InputAction::DEBUG2:
	{
		return false;
		//if (isXboxControl)
		//{
		//	input = IsXboxButtonPressed(Gamepad::Button::RIGHT_SHOULDER);
		//}
		//if (!input)
		//	input = myKeyboard->IsKeyDown(VK_F2);
		//return input;

		//break;
	}
	case InputAction::DEBUG3:
	{
		return false;
		//if (isXboxControl)
		//{
		//	input = IsXboxButtonPressed(Gamepad::Button::Y);
		//}
		//if (!input)
		//	input = myKeyboard->IsKeyDown(VK_F3);
		//return input;

		//break;
	}

	default:
		break;
	}
	return input;
}


bool InputManager::IsReleased(InputAction anAction)
{
	bool isXboxControl = false;
	bool isKeyPressed = false;
	int buttonIndex = 0;

	if (myGamePad->IsConnected())
		isXboxControl = true;

	switch (anAction)
	{
	case InputAction::DOWN:
	{
		if (isXboxControl)
		{
			isKeyPressed = myGamePad->IsButtonPressed(Gamepad::Button::DPAD_DOWN);
		}
		if (!isKeyPressed)
			isKeyPressed = (myKeyboard->IsKeyPressed(VK_DOWN));
		if (!isKeyPressed)
			isKeyPressed = (myKeyboard->IsKeyPressed('S'));
		buttonIndex = 0;
		break;
	}

	case InputAction::UP:
	{
		if (isXboxControl)
		{
			isKeyPressed = myGamePad->IsButtonPressed(Gamepad::Button::DPAD_UP);
		}
		if (!isKeyPressed)
			isKeyPressed = (myKeyboard->IsKeyPressed(VK_UP));
		if (!isKeyPressed)
			isKeyPressed = (myKeyboard->IsKeyPressed('W'));
		buttonIndex = 1;
		break;
	}
	case InputAction::LEFT:
	{
		if (isXboxControl)
		{
			isKeyPressed = myGamePad->IsButtonPressed(Gamepad::Button::DPAD_LEFT);
		}
		if (!isKeyPressed)
			isKeyPressed = (myKeyboard->IsKeyPressed(VK_LEFT));
		if (!isKeyPressed)
			isKeyPressed = (myKeyboard->IsKeyPressed('A'));
		buttonIndex = 2;
		break;
	}
	case InputAction::RIGHT:
	{
		if (isXboxControl)
		{
			isKeyPressed = myGamePad->IsButtonPressed(Gamepad::Button::DPAD_RIGHT);
		}
		if (!isKeyPressed)
			isKeyPressed = (myKeyboard->IsKeyPressed(VK_RIGHT));
		if (!isKeyPressed)
			isKeyPressed = (myKeyboard->IsKeyPressed('D'));
		buttonIndex = 3;
		break;
	}
	case InputAction::SHOOT:
	{
		if (isXboxControl)
		{
			isKeyPressed = myGamePad->IsButtonPressed(Gamepad::Button::A);
		}
		if (!isKeyPressed)
			return isKeyPressed;
		isKeyPressed = myKeyboard->IsKeyPressed('Z');
		buttonIndex = 4;
		break;
	}
	case InputAction::DASH:
	{
		if (isXboxControl)
		{
			isKeyPressed = myGamePad->IsButtonPressed(Gamepad::Button::X);
		}
		if (!isKeyPressed)
			isKeyPressed = myKeyboard->IsKeyPressed(VK_SHIFT);
		buttonIndex = 5;
		break;
	}
	case InputAction::CLIMB:
	{
		if (isXboxControl)
		{
			isKeyPressed = myGamePad->RightTrigger();
		}
		if (!isKeyPressed)
			isKeyPressed = myKeyboard->IsKeyPressed('X');
		buttonIndex = 6;
		break;
	}
	case InputAction::SELECT:
	{
		if (isXboxControl)
		{
			isKeyPressed = myGamePad->IsButtonPressed(Gamepad::Button::A);
		}
		if (!isKeyPressed)
			isKeyPressed = myKeyboard->IsKeyPressed(VK_SPACE);
		if (!isKeyPressed)
			isKeyPressed = myKeyboard->IsKeyPressed(VK_RETURN);
		buttonIndex = 7;

		break;
	}
	case InputAction::BACK:
	{
		if (isXboxControl)
		{
			isKeyPressed = myGamePad->IsButtonPressed(Gamepad::Button::B);
		}
		if (!isKeyPressed)
			isKeyPressed = myKeyboard->IsKeyPressed(VK_DELETE);
		buttonIndex = 8;


		break;
	}
	case InputAction::ESC:
	{
		if (isXboxControl)
		{
			isKeyPressed = myGamePad->IsButtonPressed(Gamepad::Button::START);
		}
		if (!isKeyPressed)
			isKeyPressed = myKeyboard->IsKeyPressed(VK_ESCAPE);
		buttonIndex = 9;
		break;
	}
	case InputAction::DEBUG1:
	{
		return false;
		//if (isXboxControl)
		//{
		//	input = IsXboxButtonReleased(Gamepad::Button::LEFT_SHOULDER);
		//}
		//if (!input)
		//	input = myKeyboard->IsKeyReleased(VK_F1);
		//break;
	}
	case InputAction::DEBUG2:
	{
		return false;
		//if (isXboxControl)
		//{
		//	input = IsXboxButtonReleased(Gamepad::Button::RIGHT_SHOULDER);
		//}
		//if (!input)
		//	input = myKeyboard->IsKeyReleased(VK_F2);
		//break;
	}
	case InputAction::DEBUG3:
	{
		return false;
		//if (isXboxControl)
		//{
		//	input = IsXboxButtonReleased(Gamepad::Button::Y);
		//}
		//if (!input)
		//	input = myKeyboard->IsKeyReleased(VK_F3);
		//break;
	}

	default:
		break;
	}


	if (!isKeyPressed && myKeysPressed[buttonIndex])
	{
		myKeysPressed[buttonIndex] = false;
		return true;
	}
	else if (isKeyPressed && !myKeysPressed[buttonIndex])
	{
		myKeysPressed[buttonIndex] = true;
	}

	return false;
}


bool InputManager::IsHeld(InputAction anAction)
{
	bool isXboxControl = false;
	bool input = false;

	if (myGamePad->IsConnected())
		isXboxControl = true;

	switch (anAction)
	{

	case InputAction::DOWN:
	{
		if (isXboxControl)
		{
			input = IsXboxButtonDown(Gamepad::Button::DPAD_DOWN);
		}
		if (!input)
			input = (myKeyboard->IsKeyPressed(VK_DOWN));
		if (!input)
			input = (myKeyboard->IsKeyPressed('S'));
		return input;

		break;
	}

	case InputAction::UP:
	{
		if (isXboxControl)
		{
			input = IsXboxButtonDown(Gamepad::Button::DPAD_UP);
		}
		if (!input)
			input = (myKeyboard->IsKeyPressed(VK_UP));
		if (!input)
			input = (myKeyboard->IsKeyPressed('W'));
		return input;

		break;
	}

	case InputAction::LEFT:
	{
		if (isXboxControl)
		{
			input = IsXboxButtonDown(Gamepad::Button::DPAD_LEFT);
		}
		if (!input)
			input = (myKeyboard->IsKeyPressed(VK_LEFT));
		if (!input)
			input = (myKeyboard->IsKeyPressed('A'));
		return input;

		break;
	}

	case InputAction::RIGHT:
	{
		if (isXboxControl)
		{
			input = IsXboxButtonDown(Gamepad::Button::DPAD_RIGHT);
		}
		if (!input)
			input = (myKeyboard->IsKeyPressed(VK_RIGHT));
		if (!input)
			input = (myKeyboard->IsKeyPressed('D'));
		return input;

		break;
	}

	case InputAction::SHOOT:
	{
		if (isXboxControl)
		{
			input = myGamePad->RightTrigger();
		}
		if (!input)
			input = myKeyboard->IsKeyPressed('Z');
		return input;

		break;
	}

	case InputAction::DASH:
	{
		if (isXboxControl)
		{
			input = IsXboxButtonDown(Gamepad::Button::X);
		}
		if (!input)
			input = myKeyboard->IsKeyPressed('X');

		return input;


		break;
	}

	case InputAction::CLIMB:
	{
		if (isXboxControl)
		{
			input = myGamePad->RightTrigger();
		}
		if (!input)
			input = myKeyboard->IsKeyPressed('Z');
		return input;

		break;
	}

	case InputAction::JUMP:
	{
		if (isXboxControl)
		{
			input = IsXboxButtonDown(Gamepad::Button::X);
		}
		if (!input)
			input = myKeyboard->IsKeyPressed('C');
		if (!input)
			input = myKeyboard->IsKeyPressed(VK_SPACE);
		return input;

		break;
	}

	case InputAction::SELECT:
	{
		if (isXboxControl)
		{
			input = IsXboxButtonDown(Gamepad::Button::A);
		}
		if (!input)
			input = myKeyboard->IsKeyPressed(VK_SPACE);
		return input;

		break;
	}

	case InputAction::BACK:
	{
		if (isXboxControl)
		{
			input = IsXboxButtonDown(Gamepad::Button::B);
		}
		if (!input)
			input = myKeyboard->IsKeyPressed(VK_DELETE);
		return input;


		break;
	}

	case InputAction::ESC:
	{
		if (isXboxControl)
		{
			input = IsXboxButtonDown(Gamepad::Button::START);
		}
		if (!input)
			input = myKeyboard->IsKeyPressed(VK_ESCAPE);
		return input;

		break;
	}

	default:
		break;
	}

	return input;
}


bool InputManager::IsScroll(InputAction aDirection)
{

	bool isXboxControl = false;
	bool input = false;
	bool shouldReturn = false;
	if (myGamePad->IsConnected())
		isXboxControl = true;

	switch (aDirection)
	{
	case InputAction::UP:
	{
		if (isXboxControl)
		{
			input = IsXboxButtonDown(Gamepad::Button::DPAD_UP);
			if (!input)
				input = (myGamePad->LeftStick().y > 0.5f);
		}
		if (!input)
			input = (myKeyboard->IsKeyPressed(VK_UP));
		if (!input)
			input = (myKeyboard->IsKeyPressed('W'));
		if (input)
		{
			if (myScrollUpTimer == 0 || myScrollUpTimer > myVerticalScrollCooldown)
			{
				myScrollUpTimer = 0.0f;
				shouldReturn = true;
			}
			myScrollUpTimer += Tga2D::Engine::GetInstance()->GetDeltaTime();
		}
		else
		{
			myScrollUpTimer = 0.0f;
		}

		break;
	}
	case InputAction::DOWN:
	{
		if (isXboxControl)
		{
			input = IsXboxButtonDown(Gamepad::Button::DPAD_DOWN);
			if (!input)
				input = (myGamePad->LeftStick().y < -0.5f);
		}
		if (!input)
			input = (myKeyboard->IsKeyPressed(VK_DOWN));
		if (!input)
			input = (myKeyboard->IsKeyPressed('S'));
		if (input)
		{
			if (myScrollDownTimer == 0 || myScrollDownTimer > myVerticalScrollCooldown)
			{
				myScrollDownTimer = 0.0f;
				shouldReturn = true;
			}
			myScrollDownTimer += Tga2D::Engine::GetInstance()->GetDeltaTime();
		}
		else
		{
			myScrollDownTimer = 0.0f;
		}

		break;
	}
	case InputAction::RIGHT:
	{
		if (isXboxControl)
		{
			input = IsXboxButtonDown(Gamepad::Button::DPAD_RIGHT);
			if (!input)
				input = (myGamePad->LeftStick().x < -0.5f);
		}
		if (!input)
			input = (myKeyboard->IsKeyPressed(VK_RIGHT));
		if (!input)
			input = (myKeyboard->IsKeyPressed('D'));
		if (input)
		{
			if (myScrollRightTimer == 0 || myScrollRightTimer > myHorizontalScrollCooldown)
			{
				myScrollRightTimer = 0.0f;
				shouldReturn = true;
			}
			myScrollRightTimer += Tga2D::Engine::GetInstance()->GetDeltaTime();
		}
		else
		{
			myScrollRightTimer = 0.0f;
		}

		break;
	}
	case InputAction::LEFT:
	{
		if (isXboxControl)
		{
			input = IsXboxButtonDown(Gamepad::Button::DPAD_LEFT);
			if (!input)
				input = (myGamePad->LeftStick().x > 0.5f);
		}
		if (!input)
			input = (myKeyboard->IsKeyPressed(VK_LEFT));
		if (!input)
			input = (myKeyboard->IsKeyPressed('A'));
		if (input)
		{
			if (myScrollLeftTimer == 0 || myScrollLeftTimer > myHorizontalScrollCooldown)
			{
				myScrollLeftTimer = 0.0f;
				shouldReturn = true;
			}
			myScrollLeftTimer += Tga2D::Engine::GetInstance()->GetDeltaTime();
		}
		else
		{
			myScrollLeftTimer = 0.0f;
		}

		break;
	}
	default:
		break;
	}

	if (shouldReturn)
		return true;

	return false;
}


//void InputManager::Vibrate()
//{
//	if (!myGamePad->IsConnected())
//		return;
//	myGamePad->Vibrate((SHORT_MAX));
//
//	myVibrateTimer.Reset().SetDuration(myVibrateDuration).Start().SetCallback([this]()
//		{
//			myGamePad->ResetVibration();
//		});
//}


Tga2D::Vector2f InputManager::MovementDir()
{
	Tga2D::Vector2f moveDir = { 0.f, 0.f };
	bool isXboxControl = false;

	if (myGamePad->IsConnected())
	{
		isXboxControl = true;
	}

	if (isXboxControl)
	{
		moveDir.x = myGamePad->LeftStick().x;
		moveDir.y = -myGamePad->LeftStick().y;
	}

	if (moveDir.x == 0 && moveDir.y == 0)
	{
		bool wantMoveRight = myKeyboard->IsKeyPressed(VK_RIGHT) || myKeyboard->IsKeyPressed('D');
		bool wantMoveLeft = myKeyboard->IsKeyPressed(VK_LEFT) || myKeyboard->IsKeyPressed('A');
		bool wantMoveDown = myKeyboard->IsKeyPressed(VK_DOWN) || myKeyboard->IsKeyPressed('S');
		bool wantMoveUp = myKeyboard->IsKeyPressed(VK_UP) || myKeyboard->IsKeyPressed('W');
		moveDir = { (float)wantMoveRight - (float)wantMoveLeft, (float)wantMoveDown - (float)wantMoveUp };
	}



	return moveDir.Normalize();
}
