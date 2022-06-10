#pragma once
#include "State.h"
#include "InputObserver.h"
#include "UIElement.h"
#include <tga2d/text/text.h>

enum eDialogueType
{
	Tutorial,
	MidPoint,
	End
};

class DialogueState : public State, public Input::InputObserver
{
public:
	DialogueState(StateStack& aStateStack, PollingStation* aPollingStation);
	~DialogueState();
	int Update(const float aDeltaTime) override;
	void Init() override;
	void Render() override;
	void RecieveEvent(const Input::eInputEvent aEvent, const float aValue) override;

private:
	int myIndex=0;
	UIElement myDialogueBox;
	UIElement myAstralProjection;
	eDialogueType myDialogueType = eDialogueType::Tutorial;
	std::vector<std::string> myDialogues;
	Tga2D::Text myText;
};