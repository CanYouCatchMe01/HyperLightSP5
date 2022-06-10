#pragma once
#include "Component.h"
#include "Observer.h"

class MeleeComponent : public Component, public Observer
{
public:
	MeleeComponent(bool aWeaponUpgrade, int someDamage);
	~MeleeComponent();
	void OnUpdate(float aDeltaTime) override;
	void TurnWeaponUpgradeOn();
	void RecieveMsg(const Message& aMsg);
	bool myWeaponUpgrade;
	int myAttackDamage;
#ifdef _DEBUG
	void DebugUpdate();
#endif // _DEBUG

	void OnAwake() override;

	void OnStart() override;

private:
};