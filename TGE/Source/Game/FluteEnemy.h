#pragma once
#include "EnemyComponent.h"
#include "json.hpp"

class FluteEnemy : public EnemyComponent
{

public:
	FluteEnemy(int aMaxHp, float aSpeed, float anAttackSpeed, float aDetectionRadius, float anIdleSpeed, float anIdleRadius, nlohmann::json& anObject);
	void OnUpdate(float aDt) override;

private:
	void MoveAwayFromPlayer(float aDt);
	void ShootPlayer(float aDT);
	void OnDeath() override;

	Tga2D::Vector3f myAttackDirection;
	int myHp;

	nlohmann::json myBullet;
	float myRunawayRadius;

	float myAttackTimer;
	bool myAttacking = false;
	bool myRunaway = false;
};