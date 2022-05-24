#include "stdafx.h"
#include "Hud.h"
#include "UIElement.h"
#include "PollingStation.h"

Hud::Hud(PollingStation* aPollingStation)
	:
	myHealthBar({ 0.12f,0.1f }, eElementType::eHealthBar, aPollingStation, { .4f, .2f }),
	myHealthKits({ 0.12f,0.1f }, eElementType::eHealthKit, aPollingStation, { .4f, .2f })
{}

Hud::~Hud()
{}

void Hud::Render()
{
	myHealthKits.Render();
	myHealthBar.Render();
}