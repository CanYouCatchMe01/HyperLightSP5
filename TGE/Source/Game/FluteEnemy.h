#pragma once
#include "EnemyComponent.h"
#include "json.hpp"
#include "Timer.h"

class FluteEnemy : public EnemyComponent
{

public:
	FluteEnemy(int aMaxHp, float aSpeed, float aDetectionRadius, float anIdleSpeed, float anIdleRadius, nlohmann::json& anObject);
	
	void OnUpdate(float aDt) override;
	void OnStart() override;
private:
	void MoveAwayFromPlayer(float aDt);
	void ShootPlayer(float aDT);
	void OnDeath() override;

	Tga2D::Vector3f myAttackDirection;
	int myHp;

	nlohmann::json myBullet;
	
	float myRunawayRadius;
	float myDoAttack = 0.f;
	
	bool myIsAttacking = false;
	bool myRunaway = false;
};