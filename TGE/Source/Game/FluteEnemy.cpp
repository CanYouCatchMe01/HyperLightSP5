#include "stdafx.h"
#include "FluteEnemy.h"
#include "Scene.h"
#include "BulletComponent.h"
#include "UnityLoader.h"

FluteEnemy::FluteEnemy(int aMaxHp, float aSpeed, float anAttackSpeed, float aDetectionRadius, float anIdleSpeed, float aRunawayRadius, nlohmann::json& anObject)
{
	myMaxHp = aMaxHp;
	mySpeed = aSpeed;
	myAttackSpeed = anAttackSpeed;
	myAttackTimer = myAttackSpeed;
	myDetectionRadius = aDetectionRadius;
	myIdleSpeed = anIdleSpeed;

	myRunawayRadius = aRunawayRadius;
	myBullet = anObject;
}

void FluteEnemy::OnUpdate(float aDt)
{
	myWalkSound->setVolume(1.f);
	myMoveTimer -= aDt;
	myTakeDamageTimer -= aDt;
	CorrectRotation(aDt);
	CheckRadius();
	
	//check if player is too close
	float r2 = myDistanceToTarget.x * myDistanceToTarget.x + myDistanceToTarget.z * myDistanceToTarget.z;
	myRunaway = r2 < myRunawayRadius * myRunawayRadius;

	if (myRunaway)
	{
		MoveAwayFromPlayer(aDt);
	}
	else if (!myIsInRange)
	{
		// do nothing
	}
	else if (myIsInRange)
	{
		ShootPlayer(aDt);
	}
}

void FluteEnemy::MoveAwayFromPlayer(float aDt)
{
	myDistanceToTarget.Normalize();
	SetPosition({ GetPosition().x - myDistanceToTarget.x * mySpeed * aDt, GetPosition().y, GetPosition().z - myDistanceToTarget.z * mySpeed * aDt });
}

void FluteEnemy::ShootPlayer(float aDt)
{
	//create timer for attacking
	myAttackTimer -= aDt;
	if (myAttackTimer <= 0)
	{
		myAttackTimer = myAttackSpeed;
		//create bullet
		GameObject* tempBullet = myScene->CreateGameObject(myBullet);
		tempBullet->GetComponent<BulletComponent>()->SetPosition({myTransform->GetPosition().x, myTransform->GetPosition().y + 1.5f, myTransform->GetPosition().z});
		tempBullet->GetComponent<BulletComponent>()->SetDirection(myDistanceToTarget);
		tempBullet->GetComponent<BulletComponent>()->OnUpdate(aDt);
	}
}

void FluteEnemy::OnDeath()
{
	myScene->RemoveGameObject(myGameObject);
	std::cout << "he dead (flute enemy)\n";
}


