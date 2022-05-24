#pragma once
#include "InputMapperEvents.h"
#include "PollingStation.h"

namespace Input
{
	class InputObserver
	{
	public:
		InputObserver() = default;
		~InputObserver();
		virtual void		RecieveEvent(const eInputEvent aEvent, const float aValue) = 0;
		void SetPollingStation(PollingStation* aPollingStation);

		std::vector<eInputEvent> myRegisteredEvents;
	private:
		PollingStation* myInputPollingStation = nullptr;
	};
}