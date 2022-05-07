#pragma once
#include "InputMapperEvents.h"
#include "tga2d/input/InputManager.h"
#include <vector>
#include <unordered_map>

namespace Input
{
	class InputObserver;

	class InputMapper
	{
	public:
		InputMapper(HWND aHWND);
		~InputMapper();

		void Update(UINT message, WPARAM wParam, LPARAM lParam);
		void MapEvent(const eInputAction aInputEvent, const eInputEvent aOutputEvent);
		void AddObserver(const eInputEvent aEventToListenFor, InputObserver* aObserver);
		void RemoveObserver(const eInputEvent aEventToListenFor, InputObserver* aObserver);

	private:

		void RunEvent(const eInputEvent aOutputEvent, const float aValue = 0);
		void TranslateActionToEvent(const eInputAction aAction, const float aValue = 0);
			 
		void UpdateKeyboardInput();

		std::unordered_map<eInputEvent, std::vector<InputObserver*>> myObservers;

		std::unordered_map<eInputAction, eInputEvent> myEvents;

		Tga2D::InputManager myInputManager;

	};
}