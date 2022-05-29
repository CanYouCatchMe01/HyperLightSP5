#include "stdafx.h"
#include "MeleeComponent.h"

MeleeComponent::MeleeComponent(bool aWeaponUpgrade, int someAttackDamage) :
	myWeaponUpgrade(aWeaponUpgrade),
	myAttackDamage(someAttackDamage)
{}

MeleeComponent::~MeleeComponent()
{
}

void MeleeComponent::OnUpdate(float aDeltaTime)
{
	aDeltaTime;
}
void MeleeComponent::TurnWeaponUpgradeOn()
{
	myWeaponUpgrade = true;
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
}
void MeleeComponent::OnStart()
{
}



