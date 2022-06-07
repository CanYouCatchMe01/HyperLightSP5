#include "stdafx.h"
#include <iostream>
#include "PopcornEnemy.h"
#include "PlayerComponent.h"
#include "GameObject.h"
#include "Scene.h"
#include "AnimatedMeshComponent.h"

PopcornEnemy::PopcornEnemy(int aMaxHp, float aSpeed, float anAttackSpeed, float aDetectionRadius, float anIdleSpeed, int anAttackDamage, float anIdleRadius)
{
	myMaxHp = aMaxHp;
	mySpeed = aSpeed;
	myAttackSpeed = anAttackSpeed;
	myDetectionRadius = aDetectionRadius;
	myIdleSpeed = anIdleSpeed;
	myAttackDmg = anAttackDamage;
	myIdleRadius = anIdleRadius;
}

void PopcornEnemy::OnUpdate(float aDt)
{
	myWalkSound->setVolume(1.f);
	myMoveTimer -= aDt;
	myTakeDamageTimer -= aDt;
	CheckRadius();

	// enemy behavior
	if (!myIsStunned && !myIsInRange)
	{
		IdleMovement(aDt);
	}
	else if (!myIsStunned && myIsInRange)
	{
		MoveTowardsPlayer(aDt);
	}
}

//void PopcornEnemy::OnStart()
//{
//	auto animatedMesh = myGameObject->GetComponent<AnimatedMeshComponent>();
//	auto wormCrawl = animatedMesh->AddTransition("sandWorm_crawl", [this]()->bool { return true; });
//	wormCrawl->AddTransition(wormCrawl, [this]()->bool { return ((myLastDir.x != 0 || myLastDir.z != 0) && !myIsStunned); });
//}


void PopcornEnemy::MoveTowardsPlayer(float aDt)
{
	myDistanceToTarget.Normalize();
	myTransform->SetRotation(myTransform->GetRotation() + myDistanceToTarget);
	SetPosition(GetPosition() + Tga2D::Vector3f{ myDistanceToTarget.x, 0, myDistanceToTarget.z } *mySpeed * aDt);
}

void PopcornEnemy::OnDeath()
{
	myScene->RemoveGameObject(myGameObject);
	std::cout << "he dead (popcorn enemy)\n";
}






