#pragma once
#include "State.h"
#include "InputObserver.h"
#include "Observer.h"
#include "Hud.h"

class GameState : public State , public Input::InputObserver, public Observer
{
public:
	GameState(StateStack& aStateStack, PollingStation* aPollingStation, const std::string& aSceneName);
	~GameState();
	int Update(const float aDeltaTime) override;
	void Init() override;
	void Render() override;
	void RecieveEvent(const Input::eInputEvent aEvent, const float aValue) override;
	void RecieveMsg(const Message& aMsg);

private:
	float myStartDialogueTimer = 0.2f;
	bool myIsTutorial = false;
	bool myIsDialogue = false;
	Hud myHud;
};