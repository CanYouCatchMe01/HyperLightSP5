#pragma once
#include "State.h"
#include "InputObserver.h"
#include "UIElement.h"
#include "Button.h"
#include <tga2d/sprite/sprite.h>
#include <vector>

enum eButtonIndex
{
	eBack = 0,
	eTPBadlands_1 = 1,
	eTPBadlands_2 = 2,
	eTPJungle_1 = 3,
	eTPJungle_2 = 4,
	eHub = 5
};

class MapState : public State, public Input::InputObserver
{
public:
	MapState(StateStack& aStateStack, PollingStation* aPollingStation);
	~MapState();
	void Init() override;
	int Update(const float aDeltaTime) override;
	void Render() override;
	void InvokeButton();
	void RecieveEvent(const Input::eInputEvent aEvent, const float aValue) override;
private:
	int myCurrentSelection;
	Tga2D::SpriteSharedData mySharedData;
	Tga2D::Sprite2DInstanceData mySpriteInstance = {};
	std::string myTeleportPoints[5];
	std::vector <Button> myButtons;
};