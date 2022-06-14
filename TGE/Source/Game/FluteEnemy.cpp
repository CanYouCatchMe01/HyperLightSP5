#include "stdafx.h"
#include "FluteEnemy.h"
#include "Scene.h"
#include "BulletComponent.h"
#include "UnityLoader.h"
#include "AnimatedMeshComponent.h"
#include "PlayerComponent.h"
#include "AudioComponent.h"

FluteEnemy::FluteEnemy(int aMaxHp, float aSpeed, float aDetectionRadius, float anIdleSpeed, float aRunawayRadius, nlohmann::json& anObject)
{
	myMaxHp = aMaxHp;
	mySpeed = aSpeed;
	myDetectionRadius = aDetectionRadius;
	myIdleSpeed = anIdleSpeed;
	myAttackSpeed = 0.75f;
	myRunawayRadius = aRunawayRadius;
	myBullet = anObject;

	myDoAttack = myAttackSpeed;
}

void FluteEnemy::OnUpdate(float aDt)
{
	myWalkSound->setVolume(1.f);
	myMoveTimer -= aDt;
	myTakeDamageTimer -= aDt;
	CorrectRotation(aDt);
	CheckRadius();
	
	if (!myIsGrounded)
	{
		myDir.y = -myGravity / mySpeed;
		SetPosition(GetPosition() + (myDir * mySpeed * aDt));
	}
	
	//check if player is too close
	float r2 = myDistanceToTarget.x * myDistanceToTarget.x + myDistanceToTarget.z * myDistanceToTarget.z;
	myRunaway = r2 < myRunawayRadius * myRunawayRadius;
	if (myPollingStation->myPlayer->GetComponent<PlayerComponent>()->myIsAlive)
	{
		if (myRunaway)
		{
			MoveAwayFromPlayer(aDt);
			myDoAttack = myAttackSpeed;
		}
		else if (!myIsInRange)
		{
			myDoAttack = myAttackSpeed;
		}
		else if (myIsInRange)
		{
			ShootPlayer(aDt);
		}
	}
}

void FluteEnemy::OnStart()
{
	auto animatedMesh = myGameObject->GetComponent<AnimatedMeshComponent>();
	if (animatedMesh)
	{
		auto fluteIdle = animatedMesh->AddTransition("Flute_Idle", [this]()->bool { return true; });
		auto fluteShoot = fluteIdle->AddTransition("Flute_Shoot", [this]()->bool { return myIsInRange; });
		auto fluteRun = fluteShoot->AddTransition("Flute_Run", [this]()->bool { return myRunaway; });
		
		fluteIdle->AddTransition(fluteShoot, [this]()->bool { return myIsInRange && !myRunaway; });
		fluteIdle->AddTransition(fluteRun, [this]()->bool { return myRunaway; });
		fluteShoot->AddTransition(fluteRun, [this]()->bool { return myRunaway; });
		fluteShoot->AddTransition(fluteIdle, [this]()->bool { return !myIsInRange; });
		fluteRun->AddTransition(fluteShoot, [this]()->bool { return myIsInRange && !myRunaway; });
		fluteRun->AddTransition(fluteIdle, [this]()->bool { return !myRunaway; });
	}
	
	myTarget = myPollingStation->myPlayer;
	myMoveTime = ((1000.f, 0.1f) * ((float)rand() / RAND_MAX)) + 0.1f;
	// need to match with the hit cooldown of the player
	myTakeDamageTime = 0.75f;
	myRandNum = -1;
}

void FluteEnemy::MoveAwayFromPlayer(float aDt)
{
	myDistanceToTarget.Normalize();
	Tga2D::Vector3f newRotation = { 0, (atan2(myDistanceToTarget.z, -myDistanceToTarget.x) + 3.14f / 2) * (180.f / 3.14f), 0 };
	myTransform->SetRotation(newRotation);
	SetPosition({ GetPosition().x - myDistanceToTarget.x * mySpeed * aDt, GetPosition().y, GetPosition().z - myDistanceToTarget.z * mySpeed * aDt });
}

void FluteEnemy::ShootPlayer(float aDt)
{
	//create timer for attacking
	myDoAttack -= aDt;
	if (myDoAttack <= 0)

	{	//Attack Audio
		myAudioComponent->PlayEvent3D(FSPRO::Event::sfx_enemy_attack_flute);
		
		myIsAttacking = true;
		myDoAttack = myAttackSpeed + 0.25f;
		//create bullet
		GameObject* tempBullet = myScene->CreateGameObject(myBullet);
		tempBullet->GetComponent<BulletComponent>()->SetPosition({myTransform->GetPosition().x, myTransform->GetPosition().y + 1.5f, myTransform->GetPosition().z});
		tempBullet->GetComponent<BulletComponent>()->SetDirection(myDistanceToTarget);
		tempBullet->GetComponent<BulletComponent>()->SetRotation(myTransform->GetRotation());
		tempBullet->GetComponent<BulletComponent>()->OnUpdate(aDt);
	}
	
	//Look at player
	myDistanceToTarget.Normalize();
	Tga2D::Vector3f newRotation = { 0, (atan2(myDistanceToTarget.z, -myDistanceToTarget.x) - 3.14f / 2) * (180.f / 3.14f), 0 };
	myTransform->SetRotation(newRotation);
}

void FluteEnemy::OnDeath()
{
	myScene->RemoveGameObject(myGameObject);
	std::cout << "he dead (flute enemy)\n";
}


