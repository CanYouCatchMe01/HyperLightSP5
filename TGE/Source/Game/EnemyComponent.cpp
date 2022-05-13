#include "stdafx.h"
#include "EnemyComponent.h"
#include "GameObject.h"
#include "MeleeComponent.h"
#include "PlayerComponent.h"

EnemyComponent::EnemyComponent()
{
}

void EnemyComponent::OnUpdate(float)
{
}

void EnemyComponent::OnAwake()
{
}

void EnemyComponent::OnStart()
{
	myTarget = myPollingStation->myPlayer;
	myMoveTime = ((1000.f, 0.1f) * ((float)rand() / RAND_MAX)) + 0.1f;
	myRandNum = -1;
}

void EnemyComponent::OnCollisionEnter(GameObject* aTrigger)
{
	if (aTrigger->GetComponent<PlayerComponent>()->myAttack)
	{
		TakeDamage(aTrigger->GetComponent<MeleeComponent>()->myDamage);
	}
}

void EnemyComponent::TakeDamage(int someDamage)
{
	myIsStunned = true;
	myHp -= someDamage;
	if (myHp <= 0) { OnDeath(); }
}

void EnemyComponent::CheckRadius()
{
	//Define a circle around the enemy
	myDistanceToTarget = myTarget->GetTransform().GetPosition() - GetPosition();
	float r2 = myDistanceToTarget.x * myDistanceToTarget.x + myDistanceToTarget.z * myDistanceToTarget.z;
	// check if player is in range
	myIsInRange = r2 < myDetectionRadius * myDetectionRadius;
}

void EnemyComponent::SetPosition(const Tga2D::Vector3f& aPosition)
{
	myTransform->SetPosition(aPosition);
}

Tga2D::Vector3f EnemyComponent::GetPosition()
{
	return myTransform->GetPosition();
}

int EnemyComponent::GetAttackDmg()
{
	return myAttackDmg;
}

void EnemyComponent::IdleMovement(float aDt)
{
	// choose a random direction or stand still.
	// choose a random time for said action to occur.
	// set position
	Tga2D::Vector3f forwardDir = myTransform->GetMatrix().GetForward();
	Tga2D::Vector3f backwardDir = forwardDir * -1.f;
	Tga2D::Vector3f rightDir = myTransform->GetMatrix().GetRight();
	Tga2D::Vector3f leftDir = rightDir * -1.f;

	if (myMoveTimer < 0.f)
	{
		myRandNum = rand() % 5;

		myMoveTime = ((4.f, 0.75f) * ((float)rand() / RAND_MAX)) + 0.75f;
		myMoveTimer = myMoveTime;
	}

	switch (myRandNum)
	{
	case 0:
		forwardDir.Normalize();
		SetPosition(GetPosition() + forwardDir * myIdleSpeed * aDt);
		break;
	case 1:
		backwardDir.Normalize();
		SetPosition(GetPosition() + backwardDir * myIdleSpeed * aDt);
		break;
	case 2:
		rightDir.Normalize();
		SetPosition(GetPosition() + rightDir * myIdleSpeed * aDt);
		break;
	case 3:
		leftDir.Normalize();
		SetPosition(GetPosition() + leftDir * myIdleSpeed * aDt);
		break;
	case 4:
		// do nothing
		SetPosition(GetPosition());
		break;
	default:
		break;
	}
}

void EnemyComponent::MoveTowardsPlayer(float aDt)
{
	myDistanceToTarget.Normalize();
	SetPosition(GetPosition() + Tga2D::Vector3f{ myDistanceToTarget.x, 0, myDistanceToTarget.z } * mySpeed * aDt);
}


