#include "stdafx.h"
#include "EnemyComponent.h"
#include "GameObject.h"

EnemyComponent::EnemyComponent(int aType, int aMaxHp, float aSpeed, float anAttackSpeed, float aDetectionRadius) : 
	myType(aType),
	myMaxHp(aMaxHp), 
	mySpeed(aSpeed), 
	myAttackSpeed(anAttackSpeed),
	myDetectionRadius(aDetectionRadius)
{}

void EnemyComponent::OnUpdate(float aDT)
{
	//Define a circle around the enemy
	Tga2D::Vector3f distance = myTarget->GetTransform().GetPosition() - GetPosition();
	float r3 = distance.x * distance.x + distance.y * distance.y + distance.z * distance.z;
	// check if player is in range
	myIsInRange = r3 < myDetectionRadius * myDetectionRadius;

	switch (myType)
	{
	case 1:
		if (distance.Length() < 1)
		{
			Charge(aDT, {distance.x, 0.f, distance.z});
		}
		else if (!myAttacking && myIsInRange)
		{
			MoveCloser(aDT, {distance.x, 0.f, distance.z});
		}
		break;
	}
	
}

void EnemyComponent::IdleMovement(float aDt)
{
	// choose a random direction or stand still.
	// choose a random time for said action to occur.
	// set position
	aDt;
	Tga2D::Vector3f forwardDir = myTransform->GetMatrix().GetForward();
	Tga2D::Vector3f backwardDir = forwardDir * -1.f;
	Tga2D::Vector3f rightDir = myTransform->GetMatrix().GetRight();
	Tga2D::Vector3f leftDir = rightDir * -1.f;

	
}

void EnemyComponent::MoveCloser(float aDT, Tga2D::Vector3f aDirection)
{
	aDirection.Normalize();
	SetPosition(GetPosition() + aDirection * mySpeed * aDT);
}

void EnemyComponent::MoveAway(float aDT, Tga2D::Vector3f aDirection)
{
	aDirection.Normalize();
	SetPosition(GetPosition() - aDirection * mySpeed * aDT);
}

void EnemyComponent::Charge(float aDT, Tga2D::Vector3f aDirection)
{
	if (myAttacking && myTelegraph < 0.0f)
	{
		SetPosition(GetPosition() + myAttackDirection * myAttackSpeed * aDT);
		//damedg
		myAttackTime -= aDT;
	}
	else if (myAttacking)
	{
		myTelegraph -= aDT;
		if (myTelegraph < 0.0f)
		{
			myAttackTime = 0.1f;
		}
	}
	else
	{
		myAttacking = true;
		myTelegraph = 0.1f;
		aDirection.Normalize();
		myAttackDirection = aDirection;
	}
}

void EnemyComponent::Attack(float /*aDT*/, Tga2D::Vector3f /*aDirection*/)
{
}

void EnemyComponent::OnAwake()
{	
}

void EnemyComponent::OnStart()
{
	myTarget = myPollingStation->myPlayer;
}

void EnemyComponent::SetPosition(Tga2D::Vector3f aPosition)
{
	myTransform->SetPosition(aPosition);
}

Tga2D::Vector3f EnemyComponent::GetPosition()
{
	return myTransform->GetPosition();
}
