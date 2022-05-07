#pragma once
#include "State.h"
#include "InputObserver.h"
#include <tga2d/sprite/sprite.h>

class Button;
enum class eButtonType;

class MainMenuState : public State, public Input::InputObserver
{
public:
	MainMenuState(StateStack& aStateStack, PollingStation* aPollingStation);
	~MainMenuState();
	void Init() override;
	PopInfo Update(const float aDeltaTime) override;
	void RecieveEvent(const Input::eInputEvent aEvent, const float aValue) override;
	void Render() override;
	void InvokeButton(eButtonType aType);

private:
	eButtonType mySelectedType;
	std::vector<Button*> myButtons;
	Tga2D::SpriteSharedData mySharedData;
	Tga2D::Sprite2DInstanceData mySpriteInstance = {};
};