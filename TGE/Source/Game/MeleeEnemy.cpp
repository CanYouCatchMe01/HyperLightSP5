#include "stdafx.h"
#include "MeleeEnemy.h"
#include "PlayerComponent.h"
#include "GameObject.h"

MeleeEnemy::MeleeEnemy(int aMaxHp, float aSpeed, float anAttackSpeed, float aDetectionRadius, float anIdleSpeed, int anAttackDamage)
{
	myMaxHp = aMaxHp;
	mySpeed = aSpeed;
	myAttackSpeed = anAttackSpeed;
	myDetectionRadius = aDetectionRadius;
	myIdleSpeed = anIdleSpeed;
	myAttackDmg = anAttackDamage;
}

void MeleeEnemy::OnUpdate(float aDt)
{
	myMoveTimer -= aDt;
	CheckRadius();

	// enemy behavior
	if (!myIsInRange)
	{
		IdleMovement(aDt);
	}
	else if (!myAttacking && myIsInRange)
	{
		MoveTowardsPlayer(aDt);
	}
}

void MeleeEnemy::OnCollisionEnter(GameObject* aOther)
{
	PlayerComponent* player = aOther->GetComponent<PlayerComponent>();
	
	if (player != nullptr)
	{
		player->TakeDamage(myAttackDmg);
	}
}

void MeleeEnemy::OnDeath()
{
}

void MeleeEnemy::MoveAway(float aDT, Tga2D::Vector3f aDirection)
{
	aDirection.Normalize();
	SetPosition(GetPosition() - aDirection * mySpeed * aDT);
}


void MeleeEnemy::Attack(float /*aDT*/, Tga2D::Vector3f /*aDirection*/)
{
}




