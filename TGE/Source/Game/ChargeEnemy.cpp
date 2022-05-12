#include "stdafx.h"
#include "ChargeEnemy.h"

ChargeEnemy::ChargeEnemy(int aMaxHp, float aSpeed, float anAttackSpeed, float aDetectionRadius)
{
	myMaxHp = aMaxHp;
	mySpeed = aSpeed;
	myAttackSpeed = anAttackSpeed;
	myDetectionRadius = aDetectionRadius;
}

void ChargeEnemy::OnUpdate(float aDt)
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

//void ChargeEnemy::Charge(float aDT, Tga2D::Vector3f aDirection)
//{
//	if (myAttacking && myTelegraph < 0.0f)
//	{
//		SetPosition(GetPosition() + myAttackDirection * myAttackSpeed * aDT);
//		//damedg
//		myAttackTime -= aDT;
//	}
//	else if (myAttacking)
//	{
//		myTelegraph -= aDT;
//		if (myTelegraph < 0.0f)
//		{
//			myAttackTime = 0.1f;
//		}
//	}
//	else
//	{
//		myAttacking = true;
//		myTelegraph = 0.1f;
//		aDirection.Normalize();
//		myAttackDirection = aDirection;
//	}
//}

void ChargeEnemy::Attack(float aDt, Tga2D::Vector3f aDirection)
{
	aDt;
	aDirection;
}
