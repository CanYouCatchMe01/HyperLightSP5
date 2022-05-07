#pragma once
#include "State.h"
#include "InputObserver.h"
#include <tga2d/sprite/sprite.h>

class Button;
enum class eButtonType;

class CreditsState : public State, public Input::InputObserver
{
public:
	CreditsState(StateStack& aStateStack, PollingStation* aPollingStation);
	~CreditsState();
	void Init() override;
	PopInfo Update(const float aDeltaTime) override;
	void RecieveEvent(const Input::eInputEvent aEvent, const float aValue) override;
	void Render() override;

private:
	Button* myBackButton;
	Tga2D::SpriteSharedData mySharedData;
	Tga2D::Sprite2DInstanceData mySpriteInstance = {};
};