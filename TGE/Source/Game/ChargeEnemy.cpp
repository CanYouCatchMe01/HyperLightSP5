#include "stdafx.h"
#include <iostream>
#include "ChargeEnemy.h"
#include "Scene.h"
#include "AnimatedMeshComponent.h"


ChargeEnemy::ChargeEnemy(int aMaxHp, float aSpeed, float anAttackSpeed, float aDetectionRadius, float aChargeRadius, float aChargeTime, float anIdleSpeed, int anAttackDamage, float aDashSpeed, float anIdleRadius)
{
	myMaxHp = aMaxHp;
	mySpeed = aSpeed;
	myAttackSpeed = anAttackSpeed;
	myDetectionRadius = aDetectionRadius;
	myChargeRadius = aChargeRadius;
	myChargeTime = aChargeTime;
	myIdleSpeed = anIdleSpeed;
	myAttackDmg = anAttackDamage;
	myDashSpeed = aDashSpeed;
	myIdleRadius = anIdleRadius;

	myChargeTimer.SetDuration(myChargeTime);
	myChargeTimer.SetCallback([this]()
		{
			myChargeDirection = myDistanceToTarget;	
			myIsDoneDashing = false;
			myDashTimer.Reset();
			myDashTimer.Start();
		});
	myDashTimer.SetDuration(0.5f);
	myDashTimer.SetCallback([this]()
		{
			myChargeTimer.Reset();
			myChargeTimer.Start();
			myIsDoneDashing = true;
		});
}

void ChargeEnemy::OnUpdate(float aDt)
{
	myChargeTimer.Update(aDt);
	myDashTimer.Update(aDt);
	myWalkSound->setVolume(1.f);
	
	myMoveTimer -= aDt;
	myTakeDamageTimer -= aDt;

	CheckRadius();
	CheckChargeRadius();

	if (!myIsInRange && !myIsInAttackRange)
	{
		myChargeTimer.Stop();
		myIsDoneDashing = false;
		IdleMovement(aDt);
	}
	else if (!myIsInAttackRange && myIsInRange)
	{
		myChargeTimer.Stop();
		myIsDoneDashing = false;
		MoveTowardsPlayer(aDt);
	}
	else if (myIsInAttackRange)
	{
		if (!myIsDoneDashing)
		{
			Charge(aDt);
		}
	}
}

void ChargeEnemy::CheckChargeRadius()
{
	float r2 = myDistanceToTarget.x * myDistanceToTarget.x + myDistanceToTarget.z * myDistanceToTarget.z;
	// check if player is in range
	myIsInAttackRange = r2 <= myChargeRadius * myChargeRadius;
}

void ChargeEnemy::Charge(float aDt)
{
	if (!myIsStunned)
	{
		myChargeTimer.Start();
		myChargeDirection.Normalize();
		SetPosition({ GetPosition().x + myChargeDirection.x * myDashSpeed * aDt, GetPosition().y, GetPosition().z + myChargeDirection.z * myDashSpeed * aDt });
		std::cout << myTransform->GetPosition().x << ", " << myTransform->GetPosition().y << ", " << myTransform->GetPosition().z << std::endl;
	}
}


void ChargeEnemy::OnDeath()
{
	myScene->RemoveGameObject(myGameObject);
	std::cout << "he dead (charge enemy)\n";
}

