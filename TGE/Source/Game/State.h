#pragma once
#include "PollingStation.h"

class StateStack;

class State
{
public:
	State(StateStack& aStateStack, PollingStation* aPollingStation);
	virtual ~State() = default;

	virtual int Update(const float aDeltaTime) = 0;
	virtual void Init() = 0;
	virtual void Render() = 0;

protected:
	int myNumberOfPops = 0;
	bool myIsActive = false;
	StateStack& myStateStack;
	PollingStation* myPollingStation;
};