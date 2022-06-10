#include "stdafx.h"
#include "MeleeComponent.h"
#include "Postmaster.h"
#include "GameDataManager.h"

MeleeComponent::MeleeComponent(bool aWeaponUpgrade, int someAttackDamage) :
	myWeaponUpgrade(aWeaponUpgrade),
	myAttackDamage(someAttackDamage)
{
}

MeleeComponent::~MeleeComponent()
{
	myPollingStation->myPostmaster.get()->RemoveObserver(this, eMessageType::eWeaponUpgrade);
}

void MeleeComponent::OnUpdate(float aDeltaTime)
{
	aDeltaTime;
}
void MeleeComponent::TurnWeaponUpgradeOn()
{
	if (myWeaponUpgrade) return;

	myAttackDamage = 2;
	myWeaponUpgrade = true;
}

void MeleeComponent::RecieveMsg(const Message& aMsg)
{
	if (aMsg.myMsg == eMessageType::eWeaponUpgrade)
	{
		TurnWeaponUpgradeOn();
	}
}

#ifdef _DEBUG
void MeleeComponent::DebugUpdate()
{
	if (ImGui::CollapsingHeader("MeleeComponent"))
	{
		ImGui::InputInt("Damage", &myAttackDamage);
		/*	ImGui::InputInt("AttackSpeed", &myAttackSpeed);*/
	}
}
#endif // _DEBUG
void MeleeComponent::OnAwake()
{
	myPollingStation->myPostmaster.get()->AddObserver(this, eMessageType::eWeaponUpgrade);
	//if (myPollingStation->myGameDataManager.get()->GetGameData().myUpgrades[1])
	//	TurnWeaponUpgradeOn();
}
void MeleeComponent::OnStart()
{
}
