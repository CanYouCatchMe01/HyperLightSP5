#include "stdafx.h"
#include "Hud.h"
#include "UIElement.h"
#include "PollingStation.h"

Hud::Hud(PollingStation* aPollingStation)
	:
	myHealthBar({ 0.12f,0.1f }, eElementType::eHealthBar, aPollingStation),
	myHealthKits({ 0.12f,0.1f }, eElementType::eHealthKit, aPollingStation)
{}

Hud::~Hud()
{}

void Hud::Render()
{
	myHealthKits.Render();
	myHealthBar.Render();
}