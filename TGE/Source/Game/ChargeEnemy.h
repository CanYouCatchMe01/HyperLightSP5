#pragma once
#include "EnemyComponent.h"
#include "Timer.h"

class ChargeEnemy : public EnemyComponent
{

public:
	ChargeEnemy(int aMaxHp, float aSpeed, float anAttackSpeed, float aDetectionRadius, float aChargeRadius, float aChargeTime, float anIdleSpeed, int anAttackDamage, float aDashSpeed, float anIdleRadius);

	void OnUpdate(float aDt) override;
	void OnStart() override;
private:
	void Charge(float aDt);
	void CheckChargeRadius();
	void OnDeath() override;

	Tga2D::Vector3f myChargeDirection;

	Timer myChargeTimer;
	Timer myDashTimer;
	Timer myAttackTimer;
	
	float myDashSpeed;
	float myChargeTime;
	float myChargeRadius;

	bool myIsDoneDashing = false;
	bool myIsInAttackRange = false;
	bool myIsAttacking = false;
	
public: 
	bool myStartDashing = false;
};