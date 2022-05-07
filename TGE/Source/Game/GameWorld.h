#pragma once
#include "PollingStation.h"
#include "StateStack.h"

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

	StateStack myStateStack;
};