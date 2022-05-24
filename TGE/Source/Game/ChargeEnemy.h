#pragma once
#include "EnemyComponent.h"
#include "Timer.h"

class ChargeEnemy : public EnemyComponent
{

public:
	ChargeEnemy(int aMaxHp, float aSpeed, float anAttackSpeed, float aDetectionRadius, float aChargeRadius, float aChargeTime, float anIdleSpeed, int anAttackDamage);
	void OnUpdate(float aDt) override;

private:
	void Charge();
	void CheckChargeRadius();
	void OnDeath() override;

	Tga2D::Vector3f myChargeDirection;
	int myHp;

	Timer myChargeTimer;
	Timer myDashTimer;

	float myChargeTime;
	float myChargeRadius;

	bool myIsDoneDashing = false;
	bool myIsInAttackRange = false;
};