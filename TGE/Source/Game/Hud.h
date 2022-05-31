#pragma once
#include <tga2d/sprite/sprite.h>
#include "UIElement.h"
#include "Observer.h"

class Hud : public Observer
{
public:
	Hud(PollingStation* aPollingStation);
	~Hud();
	void Render();
	void RecieveMsg(const Message& aMsg);
private:
	Tga2D::Vector2f myStartMultiplier = { .4f * 0.66f, .2f };
	Tga2D::Vector2f myUpgradedMultiplier = { .4f, .2f };
	Tga2D::Vector2f myCurrentMultiplier;

	PollingStation* myPollingStation;

	UIElement myHealthOutline;
	UIElement myHealthBar;
	UIElement myHealthKits;
	std::vector<std::wstring> myHealthCharges;
	int myNumberOfHealthKits = 0;
};