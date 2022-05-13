#pragma once
#include "EnemyComponent.h"

class PopcornEnemy : public EnemyComponent 
{

public:
	PopcornEnemy(int aMaxHp, float aSpeed, float anAttackSpeed, float aDetectionRadius, float anIdleSpeed, int anAttackDamage);
	void OnUpdate(float aDt) override;

private:

	/*void OnCollisionEnter(GameObject* aOther) override;*/
	void OnDeath() override;

	void MoveAway(float aDT, Tga2D::Vector3f aDirection);
	void Attack(float aDT, Tga2D::Vector3f aDirection);

	Tga2D::Vector3f myAttackDirection;
	int myHp;

	bool myAttacking = false;
};

