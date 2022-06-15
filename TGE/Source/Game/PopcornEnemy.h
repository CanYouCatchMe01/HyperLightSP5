#pragma once
#include "EnemyComponent.h"

class PopcornEnemy : public EnemyComponent 
{

public:
	PopcornEnemy(int aMaxHp, float aSpeed, float anAttackSpeed, float aDetectionRadius, float anIdleSpeed, int anAttackDamage, float anIdleRadius);

	void OnUpdate(float aDt) override;
	void OnStart() override;
	void OnAwake() override;
private:
	void OnDeath() override;
};

