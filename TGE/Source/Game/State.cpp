#include "stdafx.h"
#include "State.h"

State::State(StateStack& aStateStack, PollingStation* aPollingStation) 
	: myStateStack(aStateStack), myPollingStation(aPollingStation)
{}
