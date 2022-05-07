#pragma once
#include "PollingStation.h"

class StateStack;

struct PopInfo
{
	bool myShouldPop;
	int myNumberOfPops;
};

class State
{
public:
	State(StateStack& aStateStack, PollingStation* aPollingStation);
	virtual ~State() = default;

	virtual PopInfo Update(const float aDeltaTime) = 0;
	virtual void Init() = 0;
	virtual void Render() = 0;

protected:
	PopInfo myPopInfo;
	bool myIsActive = false;
	StateStack& myStateStack;
	PollingStation* myPollingStation;
};