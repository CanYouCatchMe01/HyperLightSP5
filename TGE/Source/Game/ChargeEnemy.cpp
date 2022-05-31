#include "stdafx.h"
#include <iostream>
#include "ChargeEnemy.h"
#include "Scene.h"



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

	myTakeDamageTimer -= aDt;

	CheckRadius();
	CheckChargeRadius();

	float yPos = GetPosition().y;
	SetPosition({ GetPosition().x, yPos -= myGravity * aDt, GetPosition().z });

	if (!myIsInRange)
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
	else if (myIsInAttackRange && myIsInRange)
	{
		if (!myIsDoneDashing)
		{
			Charge();
		}
	}
}

void ChargeEnemy::CheckChargeRadius()
{
	float r2 = myDistanceToTarget.x * myDistanceToTarget.x + myDistanceToTarget.z * myDistanceToTarget.z;
	// check if player is in range
	myIsInAttackRange = r2 <= myChargeRadius * myChargeRadius;
}

void ChargeEnemy::Charge()
{
	if (!myIsStunned)
	{
		myChargeTimer.Start();
		myChargeDirection.Normalize();
		SetPosition(GetPosition() + myChargeDirection * myDashSpeed * Tga2D::Engine::GetInstance()->GetDeltaTime());
	}
}


void ChargeEnemy::OnDeath()
{
	myScene->RemoveGameObject(myGameObject);
	std::cout << "he dead (charge enemy)\n";
}
