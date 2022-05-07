#pragma once
#include <stack>

class PollingStation;
class State;
struct PopInfo;

class StateStack
{
public:
	StateStack() = default;
	~StateStack();
	void Init(PollingStation* aPollingStation);
	void PushState(State* aState);
	void PopState();
	void RenderState();
	void RenderUnderlyingState();
	void UpdateState(const float aDeltaTime);
private:
	std::stack<State*> myStateStack;
	PollingStation* myPollingStation;
};