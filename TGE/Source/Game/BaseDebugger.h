#ifndef _RETAIL
#pragma once
#include <map>
#include <vector>
#include "PollingStation.h"

class DebugObserver;
class StateStack;

class BaseDebugger
{
public:
	BaseDebugger(PollingStation* aPollingStation);
	void AddObserver(DebugObserver* anObserver);
	void RemoveObserver(DebugObserver* anObserver);
	void DebugUpdate();

	void SetStateStack(StateStack* aStateStack);
private:
	std::vector<DebugObserver*> myObservers;
	PollingStation* myPollingStation = nullptr;
	StateStack* myStateStack;
};
#endif // _RETAIL