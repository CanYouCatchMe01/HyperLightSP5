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
	myStateStack.pop();
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
	PopInfo popInfo = myStateStack.top()->Update(aDeltaTime);

	if (popInfo.myShouldPop)
	{
		for (size_t i = 0; i < popInfo.myNumberOfPops; i++)
		{
			delete myStateStack.top();
			myStateStack.pop();
		}
	}
}
