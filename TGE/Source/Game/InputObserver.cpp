#include "stdafx.h"
#include "InputObserver.h"


Input::InputObserver::~InputObserver()
{
	if (myInputPollingStation == nullptr)
	{
		ERROR_PRINT("YOU FORGOT TO SET POLLING STATION AND NOW THE MAPPER WILL CRASH");
		return;
	}
	for (eInputEvent i : myRegisteredEvents)
	{
		myInputPollingStation->myInputMapper.get()->RemoveObserver(i, this);
	}
}

void Input::InputObserver::SetPollingStation(PollingStation* aPollingStation)
{
	myInputPollingStation = aPollingStation;
}
