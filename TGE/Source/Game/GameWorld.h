#pragma once
#include "PollingStation.h"
#include "StateStack.h"
#include "AudioManager.h"
#include <tga2d/sprite/sprite.h>

namespace Tga2D
{
	class Texture;
}

class GameWorld
{
public:
	GameWorld();
	~GameWorld();
	void Init(HWND aHWND);
	void Update(float aTimeDelta);
	void Render();
	PollingStation* myPollingStation;
private:
	float myAlpha = -0.1f;
	bool myTGAStartUp = true;
	bool myGroupStartUp = true;
	bool myFadeOut = false;
	float myStartUpTimer = 0;
	float myStartUpTime = 2.f;
	StateStack myStateStack;
	Tga2D::SpriteSharedData mySharedData;
	Tga2D::Sprite2DInstanceData mySpriteInstance = {};
};