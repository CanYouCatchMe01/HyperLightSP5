#pragma once
#include <tga2d/sprite/sprite.h>
#include "UIElement.h"

class Hud
{
public:
	Hud(PollingStation* aPollingStation);
	~Hud();
	void Render();

private:
	UIElement myHealthBar;
	UIElement myHealthKits;
};