#pragma once
#include "EnemyComponent.h"

class ChargeEnemy : public EnemyComponent
{

public:
	ChargeEnemy(int aMaxHp, float aSpeed, float anAttackSpeed, float aDetectionRadius, float anIdleSpeed, int anAttackDamage);
	void OnUpdate(float aDt) override;

private:
	//void Charge(float aDT, Tga2D::Vector3f aDirection);
	void Attack(float aDT, Tga2D::Vector3f aDirection);
	void OnDeath() override;

	Tga2D::Vector3f myAttackDirection;
	int myHp;

	float myTelegraph = 0.0f;
	float myAttackTime = 0.0f;

	bool myAttacking = false;
};