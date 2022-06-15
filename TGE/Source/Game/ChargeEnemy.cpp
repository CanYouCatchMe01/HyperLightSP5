#include "stdafx.h"
#include <iostream>
#include "ChargeEnemy.h"
#include "Scene.h"
#include "AnimatedMeshComponent.h"
#include "PlayerComponent.h"
#include "AudioComponent.h"

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
			myDistanceToTarget.Normalize();
			Tga2D::Vector3f newRotation = { 0, (atan2(myDistanceToTarget.z, -myDistanceToTarget.x) - 3.14f / 2) * (180.f / 3.14f), 0 };
			myTransform->SetRotation(newRotation);
			
			myReadyToAttack = true;
			myChargeTimer.Stop();
			myDashTimer.Reset();
			myDashTimer.Start();
		});
	myDashTimer.SetDuration(0.75f);
	myDashTimer.SetCallback([this]()
		{
			myReadyToAttack = false;
			myDashTimer.Stop();
			myAttackTimer.Reset();
			myAttackTimer.Start();
			//Attack audio
			myAudioComponent->PlayEvent3D(FSPRO::Event::sfx_enemy_attack_guitar);
		});
	
	myAttackTimer.SetDuration(1.25f);
	myAttackTimer.SetCallback([this]()
		{
			myAttackTimer.Stop();
			myChargeTimer.Reset();
			myChargeTimer.Start();
		});
	
	//mySuperUsefulAndWellThoughtOutTimer.SetDuration(0.5f);
	//mySuperUsefulAndWellThoughtOutTimer.SetCallback([this]()
	//	{
	//		myReadyToAttack = false;
	//		myAttackTimer.Reset();
	//		myAttackTimer.Start();
	//		//Attack audio
	//		myAudioComponent->PlayEvent3D(FSPRO::Event::sfx_enemy_attack_guitar);
	//	});
}

void ChargeEnemy::OnUpdate(float aDt)
{
	/*std::cout << !myIsInAttackRange && myLastDir.x != 0 && myLastDir.z != 0 << '\n';*/
	myChargeTimer.Update(aDt);
	myDashTimer.Update(aDt);
	myAttackTimer.Update(aDt);
	myWalkSound->setVolume(1.f);
	std::cout << myRandNum << '\n';

	if (!myIsGrounded)
	{
		myDir.y = -myGravity / mySpeed * aDt;
		SetPosition({ GetPosition().x, GetPosition().y - mySpeed * aDt, GetPosition().z});
	}
	
	myMoveTimer -= aDt;
	myTakeDamageTimer -= aDt;

	CheckRadius();
	CheckChargeRadius();
	if (!myPollingStation->myPlayer->GetComponent<PlayerComponent>()->myIsAlive)
	{
		IdleMovement(aDt);
	}
	else if (!myIsInRange && !myIsInAttackRange)
	{
		/*myChargeTimer.Stop();
		myDashTimer.Stop();
		myAttackTimer.Stop();*/
		IdleMovement(aDt);
	}
	else if (!myIsInAttackRange && myIsInRange && myPollingStation->myPlayer->GetComponent<PlayerComponent>()->myIsAlive)
	{
		MoveTowardsPlayer(aDt);
	}
	else if (myIsInAttackRange && myPollingStation->myPlayer->GetComponent<PlayerComponent>()->myIsAlive)
	{	
		if (myReadyToAttack)
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
		auto chargeRun = animatedMesh->AddTransition("Flute_Run", [this]()->bool { return myIdleSpeed > 0.f ; });
		auto chargeAttack = chargeRun->AddTransition("z2_enemy_charge_slash", [this]()->bool { return myIsInAttackRange; });
		
		chargeIdle->AddTransition(chargeRun, [this]()->bool { return myRandNum < 4; });
		chargeIdle->AddTransition(chargeAttack, [this]()->bool { return myIsInAttackRange; });
		chargeRun->AddTransition(chargeIdle, [this]()->bool { return myRandNum > 3; });
		chargeRun->AddTransition(chargeAttack, [this]()->bool { return myIsInAttackRange; });
		chargeAttack->AddTransition(chargeRun, [this]()->bool { return myRandNum < 4 && !myIsInAttackRange; });
		chargeAttack->AddTransition(chargeIdle, [this]()->bool { return myRandNum > 3 && !myIsInAttackRange; });
	}

	myTarget = myPollingStation->myPlayer;
	myMoveTime = ((1000.f, 0.1f) * ((float)rand() / RAND_MAX)) + 0.1f;
	// need to match with the hit cooldown of the player
	myTakeDamageTime = 0.75f;
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
		//Charge sound
		//myAudioComponent->PlayEvent3D(FSPRO::Event::sfx_enemy_charging); //Earbomb?? why?
		if(!myChargeTimer.IsOn() && !myDashTimer.IsOn() && !myAttackTimer.IsOn()){ myChargeTimer.Start(); }
		
		if (myChargeDirection.Length() != 0)
			myChargeDirection.Normalize();
		SetPosition({ GetPosition().x + myChargeDirection.x * myDashSpeed * aDt, GetPosition().y, GetPosition().z + myChargeDirection.z * myDashSpeed * aDt });
		/*std::cout << "Enemy Position: " << GetPosition() << std::endl;*/
	}
}


void ChargeEnemy::OnDeath()
{
	//Calls parent OnDeath, which should happen for every enemy
	EnemyComponent::OnDeath();

	std::cout << "he dead (charge enemy)\n";
}

