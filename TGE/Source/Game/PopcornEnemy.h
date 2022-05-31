#pragma once
#include "EnemyComponent.h"

class PopcornEnemy : public EnemyComponent 
{

public:
	PopcornEnemy(int aMaxHp, float aSpeed, float anAttackSpeed, float aDetectionRadius, float anIdleSpeed, int anAttackDamage, float anIdleRadius);
	void OnUpdate(float aDt) override;

private:
	void MoveTowardsPlayer(float aDt) override;
	void OnDeath() override;

	void MoveAway(float aDT, Tga2D::Vector3f aDirection);

	Tga2D::Vector3f myAttackDirection;
};

