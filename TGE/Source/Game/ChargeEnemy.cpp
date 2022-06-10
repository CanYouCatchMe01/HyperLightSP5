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
			myIsDoneDashing = true;
			myAttackTimer.Reset();
			myAttackTimer.Start();
		});
	
	myAttackTimer.SetDuration(1.25f);
	myAttackTimer.SetCallback([this]()
		{
			myChargeTimer.Reset();
			myChargeTimer.Start();
		});
}

void ChargeEnemy::OnUpdate(float aDt)
{
	myChargeTimer.Update(aDt);
	myDashTimer.Update(aDt);
	myAttackTimer.Update(aDt);
	myWalkSound->setVolume(1.f);
	
	myMoveTimer -= aDt;
	myTakeDamageTimer -= aDt;

	CheckRadius();
	CheckChargeRadius();

	if (!myIsInRange && !myIsInAttackRange)
	{
		myChargeTimer.Stop();
		myDashTimer.Stop();
		myAttackTimer.Stop();
		myIsDoneDashing = false;
		IdleMovement(aDt);
	}
	else if (!myIsInAttackRange && myIsInRange)
	{
		myChargeTimer.Stop();
		myDashTimer.Stop();
		myAttackTimer.Stop();
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

void ChargeEnemy::OnStart()
{
	auto animatedMesh = myGameObject->GetComponent<AnimatedMeshComponent>();
	if (animatedMesh)
	{
		auto chargeIdle = animatedMesh->AddTransition("Flute_Idle", [this]()->bool { return true; });
		auto chargeRun = chargeIdle->AddTransition("Flute_Run", [this]()->bool { return !myIsInAttackRange && myLastDir.x != 0 && myLastDir.z != 0 ; });
		auto chargeAttack = chargeRun->AddTransition("z2_enemy_charge_slash", [this]()->bool { return myIsInAttackRange; });
		
		chargeIdle->AddTransition(chargeRun, [this]()->bool { return !myIsInAttackRange && myLastDir.x != 0 && myLastDir.z != 0; });
		chargeIdle->AddTransition(chargeAttack, [this]()->bool { return myIsInAttackRange; });
		chargeRun->AddTransition(chargeIdle, [this]()->bool { return !myIsInAttackRange && myLastDir.x == 0 && myLastDir.z == 0; });
		chargeRun->AddTransition(chargeAttack, [this]()->bool { return myIsInAttackRange; });
		chargeAttack->AddTransition(chargeRun, [this]()->bool { return !myIsInAttackRange && myLastDir.x != 0 && myLastDir.z != 0; });
	}

	myTarget = myPollingStation->myPlayer;
	myMoveTime = ((1000.f, 0.1f) * ((float)rand() / RAND_MAX)) + 0.1f;
	// need to match with the hit cooldown of the player
	myTakeDamageTime = 1.f;
	myRandNum = -1;
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
	}
}


void ChargeEnemy::OnDeath()
{
	myScene->RemoveGameObject(myGameObject);
	std::cout << "he dead (charge enemy)\n";
}

