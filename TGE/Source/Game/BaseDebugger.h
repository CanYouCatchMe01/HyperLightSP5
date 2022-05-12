#ifdef _DEBUG
#pragma once
#include <map>
#include <vector>
#include "PollingStation.h"

class DebugObserver;

class BaseDebugger
{
public:
	BaseDebugger(PollingStation* aPollingStation);
	void AddObserver(DebugObserver* anObserver);
	void RemoveObserver(DebugObserver* anObserver);
	void DebugUpdate();
private:
	std::vector<DebugObserver*> myObservers;
	PollingStation* myPollingStation = nullptr;
};
#endif // _DEBUG