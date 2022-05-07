#pragma once
#include "State.h"
#include "InputObserver.h"

class GameState : public State , public Input::InputObserver
{
public:
	GameState(StateStack& aStateStack, PollingStation* aPollingStation);
	~GameState();
	PopInfo Update(const float aDeltaTime) override;
	void Init() override;
	void Render() override;
	void RecieveEvent(const Input::eInputEvent aEvent, const float aValue) override;

private:
};