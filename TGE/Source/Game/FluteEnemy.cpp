#include "stdafx.h"
#include "FluteEnemy.h"
#include "Scene.h"

FluteEnemy::FluteEnemy(int aMaxHp, float aSpeed, float anAttackSpeed, float aDetectionRadius, float anIdleSpeed, int anAttackDamage, float anIdleRadius)
{
	myMaxHp = aMaxHp;
	mySpeed = aSpeed;
	myAttackSpeed = anAttackSpeed;
	myDetectionRadius = aDetectionRadius;
	myIdleSpeed = anIdleSpeed;
	myAttackDmg = anAttackDamage;

	myIdleRadius = anIdleRadius;
}

void FluteEnemy::OnUpdate(float aDt)
{
	myWalkSound->setVolume(1.f);
	myMoveTimer -= aDt;
	myTakeDamageTimer -= aDt;
	CorrectRotation(aDt);
	CheckRadius();

	float yPos = GetPosition().y;
	SetPosition({ GetPosition().x, yPos /*-= myGravity * aDt*/, GetPosition().z });

	if (!myIsInRange)
	{
		IdleMovement(aDt);
	}
	else if (myIsInRange)
	{
		ShootPlayer(aDt, myDistanceToTarget);
	}
	//else if (!myAttacking && myIsInRange)
	//{
	//	MoveTowardsPlayer(aDt);
	//}
}

void FluteEnemy::MoveAwayFromPlayer(float aDt)
{
	myDistanceToTarget.Normalize();
	SetPosition(GetPosition() - myDistanceToTarget * mySpeed * aDt);
}

void FluteEnemy::ShootPlayer(float aDt, Tga2D::Vector3f aDirection)
{
	aDt;
	aDirection;
}

void FluteEnemy::OnDeath()
{
	myScene->RemoveGameObject(myGameObject);
	std::cout << "he dead (flute enemy)\n";
}


