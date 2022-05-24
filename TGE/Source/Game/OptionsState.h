#pragma once
#include "State.h"
#include "InputObserver.h"
#include <tga2d/sprite/sprite.h>

class Button;
enum class eButtonType;
enum class Channels;

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
	int Update(const float aDeltaTime) override;
	void RecieveEvent(const Input::eInputEvent aEvent, const float aValue) override;
	void Render() override;
	void InvokeButton(eButtonType aType);
	void VolumeChange(Channels aChannel, int aModulus);

private:
	bool myShouldChangeScreenRes=false;
	bool myFullScreen = false;
	bool myShouldChangeVolume = false;

	float myVolumeChangeSpeed;
	float myVolumeTimer = 0;

	int myScreenResIndex;
	int mySelectedButtonIndex;
	int mySelectedArrow = 100;

	eButtonType mySelectedButton;

	std::vector<Tga2D::Vector2ui> myScreenResolutions;
	std::vector<Button> myButtons;
	std::vector<Button> myArrows;

	Tga2D::SpriteSharedData mySharedData;
	Tga2D::Sprite2DInstanceData mySpriteInstance = {};
};