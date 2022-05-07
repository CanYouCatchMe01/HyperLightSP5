#pragma once
#include "State.h"
#include "InputObserver.h"
#include <tga2d/sprite/sprite.h>

class Button;
enum class eButtonType;

class PauseMenuState : public State, public Input::InputObserver
{
public:
	PauseMenuState(StateStack& aStateStack, PollingStation* aPollingStation);
	~PauseMenuState();
	PopInfo Update(const float aDeltaTime);
	void Init();
	void Render() override;
	void InvokeButton(eButtonType aType);
	void RecieveEvent(const Input::eInputEvent aEvent, const float aValue) override;

private:
	eButtonType mySelectedType;
	std::vector<Button*> myButtons;
	Tga2D::Vector3f myCameraResetPosition;
	Tga2D::SpriteSharedData mySharedData;
	Tga2D::Sprite2DInstanceData mySpriteInstance = {};
};