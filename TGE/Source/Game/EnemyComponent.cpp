#include "stdafx.h"
#include "EnemyComponent.h"
#include "GameObject.h"
#include "MeleeComponent.h"
#include "PlayerComponent.h"
#include "AudioComponent.h"
#include "BattleZone.h"

#include "Scene.h"

EnemyComponent::EnemyComponent()
{
}

EnemyComponent::~EnemyComponent()
{
}

void EnemyComponent::OnUpdate(float)
{
}

void EnemyComponent::OnAwake()
{
	myAudioComponent = myGameObject->AddComponent<AudioComponent>();
	//myWalkSound = myAudioComponent->PlayEvent3D(FSPRO::Event::sfx_enemy_walk_badlands); //sounds all the time in badlands, removing for now
	myStartPosition = GetPosition();
	myWalkSound->setVolume(0.0f);
}

void EnemyComponent::OnStart()
{
}

void EnemyComponent::SetIsGrounded(bool aIsGrounded)
 {
	myIsGrounded = aIsGrounded;
}

void EnemyComponent::OnCollisionEnter(GameObject* aTrigger)
{
	if (aTrigger->tag == eTag::gate)
	{
		myBattleZone = aTrigger->GetComponent<BattleZone>();
		myBattleZone->AddEnemy();
	}
}

void EnemyComponent::OnCollisionExit(GameObject* aOther)
{
	if (aOther->tag == eTag::gate)
	{
		aOther->GetComponent<BattleZone>()->RemoveEnemy();
		myBattleZone = nullptr;
	}
}

void EnemyComponent::OnCollisionStay(GameObject* aTrigger)
{
	if (myTakeDamageTimer < 0.f)
	{
		if (aTrigger->tag == eTag::Player)
		{
			if (aTrigger->GetComponent<PlayerComponent>()->myAttack)
			{
				//aTrigger->GetComponent<PlayerComponent>()->OnEnemyHit();
				std::cout << "enemy took damage\n";
				myTakeDamageTimer = myTakeDamageTime;
				if (aTrigger->GetComponent<MeleeComponent>() != nullptr)
				{
					TakeDamage(aTrigger->GetComponent<MeleeComponent>()->myAttackDamage);
				}
			}
		}
	}
}

void EnemyComponent::TakeDamage(int someDamage)
{
	myIsStunned = true;
	myMaxHp -= someDamage;
	if (myMaxHp <= 0) { OnDeath(); }
}

void EnemyComponent::CheckRadius()
{
	//Define a circle around the enemy
	myDistanceToTarget = myTarget->GetTransform().GetPosition() - GetPosition();
	float r2 = myDistanceToTarget.x * myDistanceToTarget.x + myDistanceToTarget.z * myDistanceToTarget.z;
	// check if player is in range
	myIsInRange = r2 < myDetectionRadius * myDetectionRadius;
}

void EnemyComponent::CorrectRotation(float aDeltaTime)
{
	if ((myDir.x != 0 || myDir.z != 0))
	{
		if (myGoalRotation != (std::atan2(myDir.z, -myDir.x) * 57.2957795f) - 90)
		{
			if (myRotation > 360)
			{
				myRotation -= 360;
			}
			else if (myRotation < -360)
			{
				myRotation += 360;
			}
			myGoalRotation = (std::atan2(myDir.z, -myDir.x) * 57.2957795f) - 90;
			myRotationDiff = myGoalRotation - myRotation;
			myRotationTime = 0;
			if (myRotationDiff > 180)
			{
				myRotationDiff = -(360 - myRotationDiff);
			}
			else if (myRotationDiff < -180)
			{
				myRotationDiff = (360 + myRotationDiff);
			}
		}
		myLastDir = myDir;
	}
	if (myRotationTime < 1 / myRotationSpeed)
	{
		myRotation += myRotationDiff * aDeltaTime * myRotationSpeed;
		myTransform->SetRotation({ 0,myRotation,0 });
		myRotationTime += aDeltaTime;
	}
	if (myRotationTime < 1 / myRotationSpeed)
	{
		myRotation += myRotationDiff * aDeltaTime * myRotationSpeed;
		myTransform->SetRotation({ 0,myRotation,0 });
		myRotationTime += aDeltaTime;
	}
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

void EnemyComponent::OnDeath()
{
	if (myBattleZone != nullptr)
		myBattleZone->RemoveEnemy();

	myScene->RemoveGameObject(myGameObject);
}

void EnemyComponent::MoveTowardsPlayer(float aDt)
{
	myDistanceToTarget.Normalize();
	Tga2D::Vector3f newRotation = { 0, (atan2(myDistanceToTarget.z, -myDistanceToTarget.x) - 3.14f / 2) * (180.f / 3.14f), 0 };

	myTransform->SetRotation(newRotation);

	SetPosition({ GetPosition().x + myDistanceToTarget.x * mySpeed * aDt, GetPosition().y, GetPosition().z + myDistanceToTarget.z * mySpeed * aDt });
}

void EnemyComponent::IdleMovement(float aDt)
{
	Tga2D::Vector3f currentPosition = GetPosition();
	float currentRadius2 = (currentPosition.x - myStartPosition.x) * (currentPosition.x - myStartPosition.x) +
		(currentPosition.z - myStartPosition.z) * (currentPosition.z - myStartPosition.z);

	Tga2D::Vector3f forwardDir = myTransform->GetMatrix().GetForward();
	forwardDir.Normalize();

	//Tga2D::Vector3f backwardDir = forwardDir * -1.f;
	//Tga2D::Vector3f rightDir = myTransform->GetMatrix().GetRight();
	//Tga2D::Vector3f leftDir = rightDir * -1.f;
	SetPosition({ currentPosition.x, currentPosition.y - (1.f * aDt), currentPosition.z });
	if (currentRadius2 > myIdleRadius * myIdleRadius)
	{
		/*std::cout << "Out of bounds" << std::endl;*/
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




