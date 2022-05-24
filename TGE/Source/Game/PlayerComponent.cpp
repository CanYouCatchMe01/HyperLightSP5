#include "stdafx.h"
#include "PlayerComponent.h"
#include <iostream>
#include "GameObject.h"
#include "EnemyComponent.h"
#include "PopcornEnemy.h"
#include "AudioComponent.h"
#include "AudioManager.h"
#include "TeleporterComponent.h"
#include "CheckPointComponent.h"

PlayerComponent::PlayerComponent(int aMaxHp, int aMaxHealing, int aMaxAttaks, float aDashTime, float aHealingtime, float aAttackTime, float aSpeed, float aDashSpeed)
{

	myMaxHp = aMaxHp;
	myPlayerData.CurrentHP = aMaxHp;
	myMaxHealing = aMaxHealing;
	myMaxAttacks = aMaxAttaks;
	myDashTime = aDashTime;
	myHealingTime = aHealingtime;
	myAttackTime = aAttackTime;
	mySpeed = aSpeed;
	myDashSpeed = aDashSpeed;
}

PlayerComponent::~PlayerComponent()
{
	myWalkSound->stop(FMOD_STUDIO_STOP_IMMEDIATE);
}

void PlayerComponent::OnUpdate(float aDT)
{
	if (!myStun && !myHealing && !myAttack)
	{
		Movement(aDT);
	}
	myStunTimer -= aDT;
	if (myStunTimer < 0)
	{
		myStun = false;
	}
	myDashTimer -= aDT;
	if (myDashTimer < 0)
	{
		myDash = false;
	}
	myAttackTimer -= aDT;
	if (myAttackTimer < 0)
	{
		myAttack = false;
	}
	myHealTimer -= aDT;
	if (myHealTimer < 0)
	{
		myHealing = false;
	}
}

void PlayerComponent::Movement(float aDT)
{

	//increasing the volume when the player is moving
	if (myDir == Tga2D::Vector3f(0.0f, 0.0f, 0.0f))
	{
		myWalkSound->setVolume(0.0f);
		return;
	}

	myWalkSound->setVolume(10.0f);
	myCheckpointSound->setVolume(10.0f);
	myDir.Normalize();
	
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
	if (myRotationTime < 1/myRotationSpeed)
	{
		myRotation += myRotationDiff * aDT*myRotationSpeed;
		myTransform->SetRotation({ 0,myRotation,0 });
		myRotationTime += aDT;
	}


	if (myDash)
	{
		SetPosition(GetPosition() + myDashDir * myDashSpeed * aDT);
		myAudioComponent->PlayEvent3D(FSPRO::Event::sfx_player_dash);
		return;
	}
	myDir.y = -myGravity/mySpeed;
	SetPosition(GetPosition() + (myDir * mySpeed * aDT));
	myDir = { 0.0f,0.0f,0.0f };
}

void PlayerComponent::Attack()
{	
	if (myAttackTimer < 0.0f)
	{
		myAttackTimer = 0.1f;
		myAttack = true;
		//do the attack
		myAudioComponent->PlayEvent3D(FSPRO::Event::sfx_player_attack);
		//myAudioComponent->PlayEvent3D("event:/sfx/player/attack");
	}
}

void PlayerComponent::PickupHealing()
{
	if (myHealingItems >= myMaxHealing)
	{
		myHp = myMaxHp;
	}
	else
	{
		myHealingItems++;
	}
}


Tga2D::Vector3f PlayerComponent::GetPosition()
{
	return myTransform->GetPosition();
}

void PlayerComponent::SetPosition(Tga2D::Vector3f aPosition)
{
	myTransform->SetPosition(aPosition);
}

int PlayerComponent::GetHp()
{
	return myHp;
}
void PlayerComponent::SetHp(int aHp)
{
	if (aHp > myMaxHp)
	{
		myHp = myMaxHp;
	}
	else
	{
		myHp = myMaxHp;
	}
}

int PlayerComponent::GetHealing()
{
	return myHealing;
}
void PlayerComponent::SetHealing(int aHealing)
{
	if (aHealing > myMaxHealing)
	{
		myHealing = myMaxHealing;
	}
	else
	{
		myHealing = aHealing;
	}
}


void PlayerComponent::RecieveEvent(const Input::eInputEvent aEvent, const float /*aValue*/)
{
	switch (aEvent)
	{
	case Input::eInputEvent::eMoveDown:
		myDir -= myCameraRotation.GetForward();
		break;
	case Input::eInputEvent::eMoveUp:
		myDir += myCameraRotation.GetForward();
		break;
	case Input::eInputEvent::eMoveRight:
		myDir += myCameraRotation.GetRight();
		break;
	case Input::eInputEvent::eMoveLeft:
		myDir -= myCameraRotation.GetRight();
		break;
	case Input::eInputEvent::eAttack:
		if (myAttackTimer < 0.0f && !myHealing)
		{
			Attack();
		}
		break;
	case Input::eInputEvent::eDash:
	{
		if (myDashTimer < -0.1 && !myHealing)
		{
			myDashTimer = myDashTime;
			myDash = true;
			if (myDir.x == 0 && myDir.y == 0)
			{
				myDashDir = myLastDir;
			}
			else
			{
				myDashDir = myDir;
			}
		}
		break;
	}
	case Input::eInputEvent::eHeal:
		if (!myHealing && !myAttack && !myDash)
		{
			if (myHealingItems > 0)
			{
				myHealing = true;
				myHealTimer = myHealingTime;
				myHealingItems--;
				myHp = myMaxHp;
			}
			else
			{
				//cry
			}
		}
		break;
	}
}


void PlayerComponent::OnAwake()
{
	myCameraRotation = myTransform->GetMatrix();
	myPollingStation->myPlayer = myGameObject;
	myPollingStation->myInputMapper.get()->AddObserver(Input::eInputEvent::eMoveDown, this);
	myPollingStation->myInputMapper.get()->AddObserver(Input::eInputEvent::eMoveLeft, this);
	myPollingStation->myInputMapper.get()->AddObserver(Input::eInputEvent::eMoveRight, this);
	myPollingStation->myInputMapper.get()->AddObserver(Input::eInputEvent::eMoveUp, this);
	myPollingStation->myInputMapper.get()->AddObserver(Input::eInputEvent::eDash, this);
	myPollingStation->myInputMapper.get()->AddObserver(Input::eInputEvent::eAttack, this);
	myPollingStation->myInputMapper.get()->AddObserver(Input::eInputEvent::eHeal, this);

	myAudioComponent = myGameObject->AddComponent<AudioComponent>();
	myWalkSound = myAudioComponent->PlayEvent3D(FSPRO::Event::sfx_player_walk_jungle);
	myCheckpointSound = myAudioComponent->PlayEvent3D(FSPRO::Event::sfx_world_checkpoint);
	SetPollingStation(myPollingStation);
}
void PlayerComponent::OnStart()
{
}
void PlayerComponent::OnCollisionEnter(GameObject* aOther)
{
	if (aOther->tag == eTag::teleporter)
	{
		aOther->GetComponent<TeleporterComponent>()->Load();
	}

	if (aOther->tag == eTag::checkpoint)
	{
		aOther->GetComponent<CheckPointComponent>()->Save();
	}

	if (aOther->tag == eTag::popcorn)
	{
		std::cout << "player took damage\n";
		TakeDamage(aOther->GetComponent<PopcornEnemy>()->GetAttackDmg());
		aOther->GetComponent<PopcornEnemy>()->myIsStunned = true;
		myAudioComponent->PlayEvent3D(FSPRO::Event::sfx_player_death);
	}
}

void PlayerComponent::OnCollisionExit(GameObject* aOther)
{
	if (aOther->tag == eTag::popcorn)
	{
		aOther->GetComponent<PopcornEnemy>()->myIsStunned = false;
	}
}

void PlayerComponent::OnDeath()
{
	if (myPlayerData.myCheckpoint != nullptr)
	{
		myPlayerData.myCheckpoint->Load(); //causes crash
	}

}

void PlayerComponent::TakeDamage(int someDamage)
{
	myHp -= someDamage;
	Message takeDmgMessage;
	takeDmgMessage.myMsg = eMessageType::ePlayerTookDMG;
	takeDmgMessage.aFloatValue = (float)myHp / (float)myMaxHp;
	myPollingStation->myPostmaster->SendMsg(takeDmgMessage);

	if (myHp <= 0) { OnDeath(); }
	//chekded
}
#ifdef _DEBUG
void PlayerComponent::DebugUpdate()
{
	if (ImGui::CollapsingHeader("PlayerComponent"))
	{
		ImGui::InputInt("HP", &myHp);
	}

}


#endif // _DEBUG


