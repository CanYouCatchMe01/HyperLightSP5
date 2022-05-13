#pragma once
#include "EnemyComponent.h"

class FluteEnemy : public EnemyComponent
{

public:
	FluteEnemy(int aMaxHp, float aSpeed, float anAttackSpeed, float aDetectionRadius);
	void OnUpdate(float aDt) override;

private:
	//void Charge(float aDT, Tga2D::Vector3f aDirection);
	void Attack(float aDT, Tga2D::Vector3f aDirection);
	void IdleMovement(float aDt) override;

	Tga2D::Vector3f myAttackDirection;
	int myHp;

	bool myAttacking = false;
};