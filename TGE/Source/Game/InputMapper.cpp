#include "stdafx.h"
#include "InputMapper.h"
#include "InputObserver.h"
#include "tga2d/input/InputManager.h"
#include <assert.h>

namespace Input
{

	InputMapper::InputMapper(HWND aHWND) : myInputManager(aHWND)
	{
	}

	InputMapper::~InputMapper()
	{
	}

	void InputMapper::RunEvent(const eInputEvent aOutputEvent, const float aValue)
	{
		int i = 0;
		for (i = 0; i < myObservers[aOutputEvent].size(); ++i)
		{
			myObservers[aOutputEvent][i]->RecieveEvent(aOutputEvent, aValue);
		}
	}

	void InputMapper::TranslateActionToEvent(const eInputAction aAction, const float aValue)
	{
		const auto eventIterator = myEvents.find(aAction);
		if (eventIterator != myEvents.end())
		{
			RunEvent(myEvents[aAction], aValue);
		}
	}

	void InputMapper::UpdateKeyboardInput()
	{
		if (myInputManager.IsKeyPressed(VK_SPACE))
		{
			TranslateActionToEvent(Input::eInputAction::eKey_Space, 1.f);
		}
		if (myInputManager.IsKeyPressed(VK_ESCAPE))
		{
			TranslateActionToEvent(Input::eInputAction::eKey_Escape, 1.f);
		}
		if (myInputManager.IsKeyPressed(VK_RETURN))
		{
			TranslateActionToEvent(Input::eInputAction::eKey_Enter, 1.f);
		}
		if (myInputManager.IsKeyPressed('X'))
		{
			TranslateActionToEvent(Input::eInputAction::eKey_X, 1.f);
		}
		if (myInputManager.IsKeyHeld(VK_UP))
		{
			TranslateActionToEvent(Input::eInputAction::eHoldArrowKeyUp, 1.f);
		}
		if (myInputManager.IsKeyHeld(VK_DOWN))
		{
			TranslateActionToEvent(Input::eInputAction::eHoldArrowKeyDown, -1.f);
		}
		if (myInputManager.IsKeyHeld(VK_RIGHT))
		{
			TranslateActionToEvent(Input::eInputAction::eHoldArrowKeyRight, 1.f);
		}
		if (myInputManager.IsKeyHeld(VK_LEFT))
		{
			TranslateActionToEvent(Input::eInputAction::eHoldArrowKeyLeft, -1.f);
		}
		if (myInputManager.IsKeyPressed(VK_UP))
		{
			TranslateActionToEvent(Input::eInputAction::ePressArrowKeyUp, 1.f);
		}
		if (myInputManager.IsKeyPressed(VK_DOWN))
		{
			TranslateActionToEvent(Input::eInputAction::ePressArrowKeyDown, -1.f);
		}
		if (myInputManager.IsKeyPressed(VK_LEFT))
		{
			TranslateActionToEvent(Input::eInputAction::ePressArrowKeyLeft, -1.f);
		}
		if (myInputManager.IsKeyPressed(VK_RIGHT))
		{
			TranslateActionToEvent(Input::eInputAction::ePressArrowKeyRight, 1.f);
		}
		if (myInputManager.IsKeyReleased(VK_UP))
		{
			TranslateActionToEvent(Input::eInputAction::eReleaseArrowUp, 1.f);
		}
		if (myInputManager.IsKeyReleased(VK_DOWN))
		{
			TranslateActionToEvent(Input::eInputAction::eReleaseArrowDown, 1.f);
		}
		if (myInputManager.IsKeyReleased(VK_LEFT))
		{
			TranslateActionToEvent(Input::eInputAction::eReleaseArrowLeft, 1.f);
		}
		if (myInputManager.IsKeyReleased(VK_RIGHT))
		{
			TranslateActionToEvent(Input::eInputAction::eReleaseArrowRight, 1.f);
		}
		if (myInputManager.IsKeyHeld('W'))
		{
			TranslateActionToEvent(Input::eInputAction::eHoldW, 1.f);
		}
		if (myInputManager.IsKeyHeld('S'))
		{
			TranslateActionToEvent(Input::eInputAction::eHoldS, -1.f);
		}
		if (myInputManager.IsKeyHeld('D'))
		{
			TranslateActionToEvent(Input::eInputAction::eHoldD, 1.f);
		}
		if (myInputManager.IsKeyHeld('A'))
		{
			TranslateActionToEvent(Input::eInputAction::eHoldA, -1.f);
		}
		if (myInputManager.IsKeyPressed('W'))
		{
			TranslateActionToEvent(Input::eInputAction::eReleaseW, 1.f);
		}
		if (myInputManager.IsKeyPressed('S'))
		{
			TranslateActionToEvent(Input::eInputAction::eReleaseS, -1.f);
		}
		if (myInputManager.IsKeyPressed('A'))
		{
			TranslateActionToEvent(Input::eInputAction::ePressArrowKeyLeft, -1.f);
		}
		if (myInputManager.IsKeyPressed('D'))
		{
			TranslateActionToEvent(Input::eInputAction::ePressArrowKeyRight, 1.f);
		}
		if (myInputManager.IsKeyReleased('W'))
		{
			TranslateActionToEvent(Input::eInputAction::eReleaseW, 1.f);
		}
		if (myInputManager.IsKeyReleased('S'))
		{
			TranslateActionToEvent(Input::eInputAction::eReleaseS, 1.f);
		}
		if (myInputManager.IsKeyReleased('A'))
		{
			TranslateActionToEvent(Input::eInputAction::eReleaseA, 1.f);
		}
		if (myInputManager.IsKeyReleased('D'))
		{
			TranslateActionToEvent(Input::eInputAction::eReleaseD, 1.f);
		}
		if (myInputManager.IsKeyPressed('M'))
		{
			TranslateActionToEvent(eInputAction::ePressM, 1.f);
		}
	}

	void InputMapper::UpdateEvents(UINT message, WPARAM wParam, LPARAM lParam)
	{
		myInputManager.UpdateEvents(message, wParam, lParam);
	}

	void InputMapper::Update()
	{
		myInputManager.Update();
		UpdateKeyboardInput();
	}

	void InputMapper::MapEvent(const eInputAction aInputAction, const eInputEvent aOutputEvent)
	{
		myEvents[aInputAction] = aOutputEvent;
	}

	void InputMapper::AddObserver(const eInputEvent aEventToListenFor, InputObserver* aObserver)
	{
		aObserver->myRegisteredEvents.push_back(aEventToListenFor);
		assert(aObserver != nullptr && "InputObsever is nullptr!");
		myObservers[aEventToListenFor].push_back(aObserver);
	}

	void InputMapper::RemoveObserver(const eInputEvent aEventToListenFor, InputObserver* aObserver)
	{
		assert(aObserver != nullptr && "InputObsever is nullptr!");
		auto it = std::find(myObservers[aEventToListenFor].begin(), myObservers[aEventToListenFor].end(), aObserver);
		if (it != myObservers[aEventToListenFor].end())
		{
			myObservers[aEventToListenFor].erase(it);
		}

	}
}