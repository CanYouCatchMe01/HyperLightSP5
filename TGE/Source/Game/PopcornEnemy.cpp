#include "stdafx.h"
#include "PopcornEnemy.h"
#include "PlayerComponent.h"
#include "GameObject.h"
#include <iostream>

PopcornEnemy::PopcornEnemy(int aMaxHp, float aSpeed, float anAttackSpeed, float aDetectionRadius, float anIdleSpeed, int anAttackDamage)
{
	myMaxHp = aMaxHp;
	mySpeed = aSpeed;
	myAttackSpeed = anAttackSpeed;
	myDetectionRadius = aDetectionRadius;
	myIdleSpeed = anIdleSpeed;
	myAttackDmg = anAttackDamage;
	
}

void PopcornEnemy::OnUpdate(float aDt)
{
	myWalkSound->setVolume(1.f);
	myMoveTimer -= aDt;
	CheckRadius();

	float yPos = GetPosition().y;
	SetPosition({ GetPosition().x, yPos -= myGravity * aDt, GetPosition().z });

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

void PopcornEnemy::OnCollisionEnter(GameObject* aOther)
{
	PlayerComponent* player = aOther->GetComponent<PlayerComponent>();
	std::cout << "that boi fine\n";
	if (player != nullptr)
	{
		std::cout << "OUCH!\n";
		player->TakeDamage(myAttackDmg);
	}
}

void PopcornEnemy::OnDeath()
{
}

void PopcornEnemy::MoveAway(float aDT, Tga2D::Vector3f aDirection)
{
	aDirection.Normalize();
	SetPosition(GetPosition() - aDirection * mySpeed * aDT);
}





