#pragma once
#include "InputMapperEvents.h"

namespace Input
{
	class InputObserver
	{
	public:
		InputObserver() = default;
		virtual				~InputObserver() = default;
		virtual void		RecieveEvent(const eInputEvent aEvent, const float aValue) = 0;

	private:

	};
}