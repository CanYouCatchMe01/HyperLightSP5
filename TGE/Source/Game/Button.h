#pragma once
#include <tga2d/sprite/sprite.h>

enum class eButtonType
{
	Start,
	Options,
	ArrowRight,
	ArrowLeft,
	MasterVol,
	MusicVol,
	SFXVol,
	Resolution,
	FullScreen,
	Credits,
	Back,
	Resume,
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
	inline eState GetState() { return myState; }
	inline eButtonType GetType() { return myButtonType; }
	inline void SetSizeMultiplier(Tga2D::Vector2f aMultiplier) { mySpriteInstance.mySizeMultiplier = aMultiplier; }
private:
	eState myState;
	eButtonType myButtonType;

	Tga2D::Texture* mySelectedTexture;
	Tga2D::Texture* myDeselectedTexture;

	Tga2D::SpriteSharedData mySharedData;
	Tga2D::Sprite2DInstanceData mySpriteInstance = {};
};