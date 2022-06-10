#include "stdafx.h"
#include "StateStack.h"
#include "State.h"

StateStack::~StateStack()
{
	while (!myStateStack.empty())
	{
		delete myStateStack.top();
		myStateStack.pop();
	}
}

void StateStack::Init(PollingStation* aPollingStation)
{
	myPollingStation = aPollingStation;
}

void StateStack::InitState()
{
	myStateStack.top()->Init();
}

void StateStack::PushState(State* aState)
{
	if (aState == nullptr)
		return;
	aState->Init();
	myStateStack.push(aState);
}

void StateStack::PopState()
{
	if (myStateStack.empty())
		return;
	delete myStateStack.top();
	myStateStack.pop();
}

State* StateStack::GetCurrentState()
{
	return myStateStack.top();
}

void StateStack::RenderState()
{
	myStateStack.top()->Render();
}

void StateStack::RenderUnderlyingState()
{
	myStateStack._Get_container()[myStateStack.size() - 2]->Render();
}

void StateStack::UpdateState(const float aDeltaTime)
{
	int popInfo = myStateStack.top()->Update(aDeltaTime);

	if (popInfo > 0)
	{
		for (size_t i = 0; i < popInfo; i++)
		{
			delete myStateStack.top();
			myStateStack.pop();
		}
	}
}
