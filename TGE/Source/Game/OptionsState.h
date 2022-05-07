#pragma once
#include "State.h"
#include "InputObserver.h"
#include <tga2d/sprite/sprite.h>

class Button;
enum class eButtonType;
enum class eArrowIndex
{
	eResDown,
	eResUp,
	eMasterDown,
	eMasterUp,
	eMusicDown,
	eMusicUp,
	eSFXDown,
	eSFXUp
};
class OptionsState : public State, public Input::InputObserver
{
public:
	OptionsState(StateStack& aStateStack, PollingStation* aPollingStation);
	~OptionsState();
	void Init() override;
	PopInfo Update(const float aDeltaTime) override;
	void RecieveEvent(const Input::eInputEvent aEvent, const float aValue) override;
	void Render() override;
	void InvokeButton(eButtonType aType);

private:
	bool myFullScreen = false;
	std::vector<Button*> myButtons;
	std::vector<Button*> myArrows;
	eButtonType mySelectedButton;
	int mySelectedArrow = 100;
	Tga2D::SpriteSharedData mySharedData;
	Tga2D::Sprite2DInstanceData mySpriteInstance = {};
};