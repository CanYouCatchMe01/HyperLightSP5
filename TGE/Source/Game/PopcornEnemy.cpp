#include "stdafx.h"
#include "PopcornEnemy.h"
#include "PlayerComponent.h"
#include "GameObject.h"
#include <iostream>

PopcornEnemy::PopcornEnemy(int aMaxHp, float aSpeed, float anAttackSpeed, float aDetectionRadius, float anIdleSpeed, int anAttackDamage)
{
	myMaxHp = aMaxHp;
	mySpeed = aSpeed;
	myAttackSpeed = anAttackSpeed;
	myDetectionRadius = aDetectionRadius;
	myIdleSpeed = anIdleSpeed;
	myAttackDmg = anAttackDamage;
	
}

void PopcornEnemy::OnUpdate(float aDt)
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

//void PopcornEnemy::OnCollisionEnter(GameObject* aOther)
//{
//}

void PopcornEnemy::OnDeath()
{
}

void PopcornEnemy::MoveAway(float aDT, Tga2D::Vector3f aDirection)
{
	aDirection.Normalize();
	SetPosition(GetPosition() - aDirection * mySpeed * aDT);
}


void PopcornEnemy::Attack(float /*aDT*/, Tga2D::Vector3f /*aDirection*/)
{
}




