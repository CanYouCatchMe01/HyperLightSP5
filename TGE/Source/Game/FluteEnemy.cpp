#include "stdafx.h"
#include "FluteEnemy.h"

FluteEnemy::FluteEnemy(int aMaxHp, float aSpeed, float anAttackSpeed, float aDetectionRadius, float anIdleSpeed, int anAttackDamage)
{
	myMaxHp = aMaxHp;
	mySpeed = aSpeed;
	myAttackSpeed = anAttackSpeed;
	myDetectionRadius = aDetectionRadius;
	myIdleSpeed = anIdleSpeed;
	myAttackDmg = anAttackDamage;

	myIdleRadius = 1.0f;
}

void FluteEnemy::OnUpdate(float aDt)
{
	myWalkSound->setVolume(1.f);
	myMoveTimer -= aDt;
	myTakeDamageTimer -= aDt;
	CheckRadius();

	float yPos = GetPosition().y;
	SetPosition({ GetPosition().x, yPos /*-= myGravity * aDt*/, GetPosition().z });

	if (!myIsInRange)
	{
		IdleMovement(aDt);
	}
	else if (myIsInRange)
	{
		Attack(aDt, myDistanceToTarget);
	}
	//else if (!myAttacking && myIsInRange)
	//{
	//	MoveTowardsPlayer(aDt);
	//}
}

void FluteEnemy::OnAwake()
{
	myStartPosition = GetPosition();
}

void FluteEnemy::Attack(float aDt, Tga2D::Vector3f aDirection)
{
	aDt;
	aDirection;
}

void FluteEnemy::OnDeath()
{

}

void FluteEnemy::IdleMovement(float aDt)
{
	Tga2D::Vector3f currentPosition = GetPosition();
	float currentRadius2 = (currentPosition.x - myStartPosition.x) * (currentPosition.x - myStartPosition.x) +
		(currentPosition.z - myStartPosition.z) * (currentPosition.z - myStartPosition.z);

	Tga2D::Vector3f forwardDir = myTransform->GetMatrix().GetForward();
	forwardDir.Normalize();

	//Tga2D::Vector3f backwardDir = forwardDir * -1.f;
	//Tga2D::Vector3f rightDir = myTransform->GetMatrix().GetRight();
	//Tga2D::Vector3f leftDir = rightDir * -1.f;
	if (currentRadius2 > myIdleRadius * myIdleRadius)
	{
		std::cout << "Out of bounds" << std::endl;
		std::vector<Tga2D::Vector3f> dirs = { myTransform->GetMatrix().GetForward(),//forward
									  myTransform->GetMatrix().GetForward() * -1.f,	//backwards
									  myTransform->GetMatrix().GetRight(),			//right
									  myTransform->GetMatrix().GetRight() * -1.f };	//left

		Tga2D::Vector3f currentDiff = myStartPosition - currentPosition;

		Tga2D::Vector3f closest;
		for (Tga2D::Vector3f& dir : dirs)
		{
			if (dir.Dot(currentDiff) > closest.Dot(currentDiff))
				closest = dir;
		}
		if (!myHasTurned)
		{
			std::cout << "Turned" << std::endl;
			float rotation = atan2(closest.x, closest.z) * 57.2f;
			myTransform->SetRotation({ 0,rotation,0 });
			myHasTurned = true;
		}

		SetPosition(GetPosition() + closest * myIdleSpeed * aDt);
		return;
	}
	else if (myMoveTimer < 0.f)
	{
		myHasTurned = false;
		myRandNum = rand() % 6;

		myMoveTime = ((4.f, 0.75f) * ((float)rand() / RAND_MAX)) + 0.75f;
		myMoveTimer = myMoveTime;
	}

	Tga2D::Rotator currentRotation = myTransform->GetRotation();

	switch (myRandNum)
	{
	case 0:
		if (!myHasTurned)
			myHasTurned = true;
		SetPosition(GetPosition() + forwardDir * myIdleSpeed * aDt);
		break;
	case 1:
		if (!myHasTurned)
		{
			myTransform->SetRotation({ 0, currentRotation.y - 180,0 });
			myHasTurned = true;
		}
		SetPosition(GetPosition() + forwardDir * myIdleSpeed * aDt);
		break;
	case 2:
		if (!myHasTurned)
		{
			myTransform->SetRotation({ 0, currentRotation.y + 90,0 });
			myHasTurned = true;
		}
		SetPosition(GetPosition() + forwardDir * myIdleSpeed * aDt);
		break;
	case 3:
		if (!myHasTurned)
		{
			myTransform->SetRotation({ 0, currentRotation.y - 90,0 });
			myHasTurned = true;
		}
		SetPosition(GetPosition() + forwardDir * myIdleSpeed * aDt);
		break;
	case 4: //slightly higher chance to stand still
	case 5:
		// do nothing
		SetPosition(GetPosition());
		myWalkSound->setVolume(0.f);
		break;
	default:
		break;
	}
}
