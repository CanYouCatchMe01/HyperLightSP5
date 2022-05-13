#include "stdafx.h"
#include "FluteEnemy.h"

FluteEnemy::FluteEnemy(int aMaxHp, float aSpeed, float anAttackSpeed, float aDetectionRadius, float anIdleSpeed, int anAttackDamage)
{
	myMaxHp = aMaxHp;
	mySpeed = aSpeed;
	myAttackSpeed = anAttackSpeed;
	myDetectionRadius = aDetectionRadius;
	myIdleSpeed = anIdleSpeed;
	myAttackDmg = anAttackDamage;
}

void FluteEnemy::OnUpdate(float aDt)
{
	myMoveTimer -= aDt;
	CheckRadius();

	if (!myIsInRange)
	{
		IdleMovement(aDt);
	}
	else if (!myAttacking && myIsInRange)
	{
		MoveTowardsPlayer(aDt);
	}
}

void FluteEnemy::Attack(float aDt, Tga2D::Vector3f aDirection)
{
	aDt;
	aDirection;
}

void FluteEnemy::OnDeath()
{
}

void FluteEnemy::IdleMovement(float aDt)
{
	aDt;
	// maybe slight movement
}
