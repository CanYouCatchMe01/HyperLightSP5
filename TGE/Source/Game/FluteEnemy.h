#pragma once
#include "EnemyComponent.h"

class FluteEnemy : public EnemyComponent
{

public:
	FluteEnemy(int aMaxHp, float aSpeed, float anAttackSpeed, float aDetectionRadius, float anIdleSpeed, int anAttackDamage, float anIdleRadius);
	void OnUpdate(float aDt) override;

private:
	void MoveAwayFromPlayer(float aDt);
	void ShootPlayer(float aDT, Tga2D::Vector3f aDirection);
	void OnDeath() override;

	Tga2D::Vector3f myAttackDirection;
	int myHp;

	bool myAttacking = false;
};