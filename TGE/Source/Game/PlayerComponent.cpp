#include "stdafx.h"
#include "PlayerComponent.h"
#include <iostream>
#include "GameObject.h"
#include "EnemyComponent.h"
#include "PopcornEnemy.h"
#include "ChargeEnemy.h"
#include "FluteEnemy.h"
#include "AudioComponent.h"
#include "AudioManager.h"
#include "TeleporterComponent.h"
#include "TeleportActivator.h"
#include "CheckPointComponent.h"
#include "AnimatedMeshComponent.h"
#include "MusicParameterComponent.h"

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

void PlayerComponent::OnUpdate(float aDt)
{
	myTakeDamageTimer -= aDt;

	if (!myStun && !myHealing && !myAttack)
	{
		Movement(aDt);
		myLastDir = myDir;
		if (myDash && myDir.Y < 0)
		{
			myDash;
		}
		myDir = { 0.0f,0.0f,0.0f };
	}
	myStunTimer -= aDt;
	if (myStunTimer < 0)
	{
		myStun = false;
	}
	myDashTimer -= aDt;
	if (myDashTimer < 0)
	{
		myDash = false;
	}
	myAttackTimer -= aDt;
	if (myAttackTimer < 0)
	{
		myAttack = false;
	}
	myHealTimer -= aDt;
	if (myHealTimer < 0)
	{
		myHealing = false;
	}
}

void PlayerComponent::Movement(float aDT)
{

	//increasing the volume when the player is moving
	if (myDir.x == 0 && myDir.z == 0)
	{
		//Commented out for game testing//
		//myDir.y = -myGravity / mySpeed;
		//SetPosition(GetPosition() + (myDir * aDT));
		myWalkSound->setVolume(0.0f);
		return;
	}
	myWalkSound->setVolume(10.0f);
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
		return;
	}
	myDir.y = -myGravity / mySpeed;
	SetPosition(GetPosition() + (myDir * mySpeed * aDT));


}

void PlayerComponent::Attack()
{	
	if (myAttackTimer < 0.0f)
	{
		myAttackTimer = 0.2f;
		myAttack = true;
		//do the attack
		myAudioComponent->PlayEvent3D(FSPRO::Event::sfx_player_attack);
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

void PlayerComponent::SetFullHP()
{
	myHp = myMaxHp;

	Message takeDmgMessage;
	takeDmgMessage.myMsg = eMessageType::ePlayerTookDMG;
	takeDmgMessage.aFloatValue = (float)myHp / (float)myMaxHp;
	myPollingStation->myPostmaster->SendMsg(takeDmgMessage);
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
	if (myDir.y < 0)
	{
		myDir;
	}
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
		if (myDashTimer < 0 && !myHealing)
		{
			myDashTimer = myDashTime;
			myDash = true;
			if (myLastDir.x != 0 || myLastDir.z != 0)
			{
				myAudioComponent->PlayEvent3D(FSPRO::Event::sfx_player_dash);
				myDashDir.x = myLastDir.x;
				myDashDir.z = myLastDir.z;
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
	myWalkSound->setVolume(0.0f);
	SetPollingStation(myPollingStation);
}
void PlayerComponent::OnStart()
{
	myTakeDamageTime = 1.f;
	auto animatedMesh = myGameObject->GetComponent<AnimatedMeshComponent>();
	auto playerIdle = animatedMesh->AddTransition("Player_Idle", [this]()->bool {return true; });
	auto playerRun = playerIdle->AddTransition("Player_Run", [this]()->bool {return (myLastDir.x != 0 || myLastDir.z != 0); });
	auto playerDash = playerRun->AddTransition("Player_Dash", [this]()->bool {return myDash; });
	playerRun->AddTransition(playerIdle, [this]()->bool {return (myLastDir.x == 0 && myLastDir.z == 0); });
	playerDash->AddTransition(playerIdle, [this]()->bool {return (myLastDir.x == 0 && myLastDir.z == 0 && !myDash); });
	playerDash->AddTransition(playerRun, [this]()->bool {return ((myLastDir.x != 0 || myLastDir.z != 0) && !myDash); });
}

void PlayerComponent::OnCollisionEnter(GameObject* aOther)
{
	if (aOther->tag == eTag::teleporter)
	{
		aOther->GetComponent<TeleporterComponent>()->Load();
		return;
	}

	if (aOther->tag == eTag::teleportActivator)
	{
		aOther->GetComponent<TeleportActivator>()->Activate();
		return;
	}

	if (aOther->tag == eTag::music_change_parameter)
	{
		aOther->GetComponent<MusicParameterComponent>()->ChangeParameter();
		return;
	}

	if (aOther->tag == eTag::checkpoint)
	{
		aOther->GetComponent<CheckPointComponent>()->Save();
	}
	if (myTakeDamageTimer < 0.f)
	{
		if (aOther->tag == eTag::popcorn)
		{
			if (aOther->GetComponent<PopcornEnemy>() != nullptr)
			{
				myTakeDamageTimer = myTakeDamageTime;
				std::cout << "player took damage by popcorn\n";
				TakeDamage(aOther->GetComponent<PopcornEnemy>()->GetAttackDmg());
				aOther->GetComponent<PopcornEnemy>()->myIsStunned = true;
				myAudioComponent->PlayEvent3D(FSPRO::Event::sfx_player_death);
			}
		}

		if (aOther->tag == eTag::charge)
		{
			if (aOther->GetComponent<ChargeEnemy>() != nullptr)
			{
				myTakeDamageTimer = myTakeDamageTime;
				std::cout << "player took damage by charge\n";
				TakeDamage(aOther->GetComponent<ChargeEnemy>()->GetAttackDmg());
				aOther->GetComponent<ChargeEnemy>()->myIsStunned = true;
				myAudioComponent->PlayEvent3D(FSPRO::Event::sfx_player_death);
			}
		}

		if (aOther->tag == eTag::flute)
		{
			if (aOther->GetComponent<FluteEnemy>() != nullptr)
			{
				myTakeDamageTimer = myTakeDamageTime;
				std::cout << "player took damage by flute\n";
				TakeDamage(aOther->GetComponent<FluteEnemy>()->GetAttackDmg());

				myAudioComponent->PlayEvent3D(FSPRO::Event::sfx_player_death);
			}
		}
	}
}

void PlayerComponent::OnCollisionExit(GameObject* aOther)
{
	if (aOther->tag == eTag::popcorn)
	{
		if (aOther->GetComponent<PopcornEnemy>() != nullptr) 
		{
			aOther->GetComponent<PopcornEnemy>()->myIsStunned = false;
		}
	}

	if (aOther->tag == eTag::charge)
	{
		if (aOther->GetComponent<ChargeEnemy>() != nullptr) 
		{
			aOther->GetComponent<ChargeEnemy>()->myIsStunned = false;
		}
	}

	if (aOther->tag == eTag::flute)
	{
		if (aOther->GetComponent<FluteEnemy>() != nullptr)
		{
			aOther->GetComponent<FluteEnemy>()->myIsStunned = false;
		}
	}
}

void PlayerComponent::OnDeath()
{
	std::cout << "player died\n";
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


