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

	if (!myIsGrounded)
	{
		myDir.y = -myGravity / mySpeed;
		SetPosition(GetPosition() + (myDir * mySpeed * aDt));
	}
	
	// enemy behavior
	if (!myIsStunned && !myIsInRange)
	{
		IdleMovement(aDt);
	}
	else if (!myIsStunned && myIsInRange && myPollingStation->myPlayer->GetComponent<PlayerComponent>()->myIsAlive)
	{
		MoveTowardsPlayer(aDt);
	}
}

void PopcornEnemy::OnStart()
{
	auto animatedMesh = myGameObject->GetComponent<AnimatedMeshComponent>();
	if (animatedMesh)
	{
		animatedMesh->AddTransition("sandWorm_crawl", [this]()->bool { return true; });
		animatedMesh;
	}
	
	myTarget = myPollingStation->myPlayer;
	myMoveTime = ((1000.f, 0.1f) * ((float)rand() / RAND_MAX)) + 0.1f;
	// need to match with the hit cooldown of the player
	myTakeDamageTime = 0.75f;
	myRandNum = -1;
	
}

void PopcornEnemy::OnDeath()
{
	myScene->RemoveGameObject(myGameObject);
	std::cout << "he dead (popcorn enemy)\n";
}






