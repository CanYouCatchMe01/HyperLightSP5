#pragma once
#include "Component.h"


class MeleeComponent : public Component
{
public:
	MeleeComponent(bool aWeaponUpgrade, int someDamage);
	~MeleeComponent();
	void OnUpdate(float aDeltaTime) override;
	void TurnWeaponUpgradeOn();
	bool myWeaponUpgrade;
	int myAttackDamage;
#ifdef _DEBUG
	void DebugUpdate();
#endif // _DEBUG

	void OnAwake() override;

	void OnStart() override;

private:
};