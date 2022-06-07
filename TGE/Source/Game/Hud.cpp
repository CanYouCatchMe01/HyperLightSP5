#include "stdafx.h"
#include "Hud.h"
#include "UIElement.h"
#include "PollingStation.h"
#include "Postmaster.h"

Hud::Hud(PollingStation* aPollingStation)
	:
	myHealthOutline({ 0.12f,0.1f }, myUpgradedMultiplier, L"Sprites/UI/HUD/ui_hud_healthBar_frame_short.dds"),
	myHealthBar({ 0.12f,0.1f }, myStartMultiplier, L"Sprites/UI/HUD/ui_hud_healthBar_life.dds"),
	myHealthKits({ 0.12f,0.1f }, myUpgradedMultiplier, L"Sprites/UI/HUD/ui_hud_healingCharges_0.dds"),
	myPollingStation(aPollingStation)
{
	float healthBarFillVariable1 = 2.28f / myStartMultiplier.x;
	float healthBarFillVariable2 = (myStartMultiplier.x) / (1.f / 3.f);

	myHealthBar.SetPivot({ (myStartMultiplier.x / healthBarFillVariable2), 0.5f });
	myHealthBar.SetPosition({ 0.12f - ((myStartMultiplier.x / healthBarFillVariable2) / healthBarFillVariable1), 0.1f });

	myCurrentMultiplier = myStartMultiplier;
	myHealthCharges.push_back(L"Sprites/UI/HUD/ui_hud_healingCharges_0.dds");
	myHealthCharges.push_back(L"Sprites/UI/HUD/ui_hud_healingCharges_1.dds");
	myHealthCharges.push_back(L"Sprites/UI/HUD/ui_hud_healingCharges_2.dds");
	myHealthCharges.push_back(L"Sprites/UI/HUD/ui_hud_healingCharges_3.dds");

	myPollingStation->myPostmaster->AddObserver(this, eMessageType::ePlayerTookDMG);
	myPollingStation->myPostmaster->AddObserver(this, eMessageType::eHealthUpgrade);
	myPollingStation->myPostmaster->AddObserver(this, eMessageType::ePlayerHealed);
	myPollingStation->myPostmaster->AddObserver(this, eMessageType::ePlayerPickedUpHealth);
}

void Hud::RecieveMsg(const Message& aMsg)
{
	float healthValue = aMsg.aFloatValue;
	switch (aMsg.myMsg)
	{
	case eMessageType::ePlayerHealed:
		if (myNumberOfHealthKits > 0)
			myNumberOfHealthKits--;

		myHealthKits.ChangeTexture(myHealthCharges[myNumberOfHealthKits]);
		myHealthBar.ChangeSizeMultiplier({ myCurrentMultiplier.x * aMsg.aFloatValue, myCurrentMultiplier.y });

		break;

	case eMessageType::ePlayerPickedUpHealth:
		if (myNumberOfHealthKits < 3)
			myNumberOfHealthKits++;

		myHealthKits.ChangeTexture(myHealthCharges[myNumberOfHealthKits]);

		break;
	case eMessageType::ePlayerTookDMG:
		if (healthValue < 0)
			healthValue = 0;

		myHealthBar.ChangeSizeMultiplier({ myCurrentMultiplier.x * healthValue, myCurrentMultiplier.y });

		break;
	case eMessageType::eHealthUpgrade:
		myCurrentMultiplier = myUpgradedMultiplier;
		myHealthBar.ChangeSizeMultiplier(myCurrentMultiplier);
		myHealthOutline.ChangeTexture(L"Sprites/UI/HUD/ui_hud_healthBar_frame.dds");

		break;
	default:
		break;
	}
}

Hud::~Hud()
{
	myPollingStation->myPostmaster->RemoveObserver(this, eMessageType::ePlayerTookDMG);
	myPollingStation->myPostmaster->RemoveObserver(this, eMessageType::eHealthUpgrade);
	myPollingStation->myPostmaster->RemoveObserver(this, eMessageType::ePlayerHealed);
	myPollingStation->myPostmaster->RemoveObserver(this, eMessageType::ePlayerPickedUpHealth);
}

void Hud::Render()
{
	myHealthBar.Render();
	myHealthOutline.Render();
	myHealthKits.Render();
}