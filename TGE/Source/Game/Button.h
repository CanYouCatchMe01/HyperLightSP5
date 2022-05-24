#pragma once
#include <tga2d/sprite/sprite.h>
#include "tga2d/engine.h"
#include <tga2d/text/text.h>

enum class eButtonType
{
	Start,
	Options,
	ArrowRight,
	ArrowLeft,
	FullScreen,
	Resolution,
	MasterVol,
	MusicVol,
	SFXVol,
	Credits,
	Back,
	MapBack,
	Resume,
	Teleport,
	Hub,
	ExitToMain,
	Exit,
};

enum class eState
{
	None,
	Selected
};

class Button
{
public:
	Button(eButtonType aButtonType, Tga2D::Vector2f aPosition);
	void SetState(eState aState);
	void Render();
	void SetActiveColour();
	void ResetColour();
	inline eState GetState() { return myState; }
	inline eButtonType GetType() { return myButtonType; }
	inline void SetSizeMultiplier(Tga2D::Vector2f aMultiplier) { mySpriteInstance.mySizeMultiplier = aMultiplier; }
	void ToggleFullScreen();
	void SetText(std::string aText);
	void SetActiveTP(bool aStatus);
private:
	bool myIsTeleport = false;
	bool myHasText = false;
	Tga2D::Text myText;
	eState myState;
	eButtonType myButtonType;

	Tga2D::Texture* mySelectedTexture;
	Tga2D::Texture* myDeselectedTexture;

	Tga2D::Texture* myInactiveTexture;
	Tga2D::Texture* myActiveTexture;

	Tga2D::SpriteSharedData mySharedData;
	Tga2D::Sprite2DInstanceData mySpriteInstance = {};
};