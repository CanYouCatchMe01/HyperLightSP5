#include "stdafx.h"
#include "MeleeEnemy.h"
#include "GameObject.h"
#include <random>
#include <chrono>
#include <ctime>

MeleeEnemy::MeleeEnemy(int aMaxHp, float aSpeed, float anAttackSpeed, float aDetectionRadius)
{
	myMaxHp = aMaxHp;
	mySpeed = aSpeed;
	myAttackSpeed = anAttackSpeed;
	myDetectionRadius = aDetectionRadius;
}

void MeleeEnemy::OnUpdate(float aDt)
{
	myMoveTimer -= aDt;
	CheckRadius();

	// enemy behavior
	if (!myIsInRange)
	{
		IdleMovement(aDt);
	}
	else if (!myAttacking && myIsInRange)
	{
		MoveTowardsPlayer(aDt);
	}
}

void MeleeEnemy::MoveAway(float aDT, Tga2D::Vector3f aDirection)
{
	aDirection.Normalize();
	SetPosition(GetPosition() - aDirection * mySpeed * aDT);
}


void MeleeEnemy::Attack(float /*aDT*/, Tga2D::Vector3f /*aDirection*/)
{
}




