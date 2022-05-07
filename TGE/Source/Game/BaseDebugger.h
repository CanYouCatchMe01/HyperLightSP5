#pragma once
#include <map>
#include <vector>

class DebugObserver;

class BaseDebugger
{
public:
	void AddObserver(DebugObserver* anObserver);
	void RemoveObserver(DebugObserver* anObserver);
	void DebugUpdate();
private:
	std::vector<DebugObserver*> myObservers;
};