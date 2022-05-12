#pragma once
#include "EnemyComponent.h"

class MeleeEnemy : public EnemyComponent
{

public:
	MeleeEnemy(int aMaxHp, float aSpeed, float anAttackSpeed, float aDetectionRadius);
	void OnUpdate(float aDt) override;
	
private:
	void MoveAway(float aDT, Tga2D::Vector3f aDirection);

	void Attack(float aDT, Tga2D::Vector3f aDirection);

	Tga2D::Vector3f myAttackDirection;
	int myHp;

	bool myAttacking = false;
};

