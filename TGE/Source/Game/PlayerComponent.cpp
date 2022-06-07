#include "stdafx.h"
#include "PlayerComponent.h"
#include <iostream>
#include "GameObject.h"
#include "EnemyComponent.h"
#include "PopcornEnemy.h"
#include "ChargeEnemy.h"
#include "BulletComponent.h"
#include "AudioComponent.h"
#include "AudioManager.h"
#include "TeleporterComponent.h"
#include "TeleportActivator.h"
#include "CheckPointComponent.h"
#include "AnimatedMeshComponent.h"
#include "MusicParameterComponent.h"
#include "Scene.h"
#include "GameDataManager.h"
#include "SceneManager.h"
#include "CassetTapeComponent.h"
#include "GameDataManager.h"
#include "CollisionManager.h"

PlayerComponent::PlayerComponent(int aMaxHp, int aMaxHealing, int aMaxAttaks, float aDashTime, float aHealingtime, float aAttackTime, float aSpeed, float aDashSpeed)
{

	myPlayerData.myMaxHP = aMaxHp;
	myPlayerData.myCurrentHP = aMaxHp;
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
		/*myDir.y = -myGravity / mySpeed;
		SetPosition(GetPosition() + (myDir * aDT));*/
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
		myAudioComponent->PlayEvent3D(FSPRO::Event::sfx_player_swing);
	}
}

void PlayerComponent::PickupHealing()
{
	if (myHealingItems >= myMaxHealing)
	{
		myHealingItems = myMaxHealing;
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
	return myPlayerData.myCurrentHP;
}
void PlayerComponent::SetHp(int aHp)
{
	if (aHp > myPlayerData.myMaxHP)
	{
		myPlayerData.myCurrentHP = myPlayerData.myMaxHP;
	}
	else
	{
		myPlayerData.myCurrentHP = myPlayerData.myMaxHP;
	}
}

void PlayerComponent::SetFullHP()
{
	myPlayerData.myCurrentHP = myPlayerData.myMaxHP;

	Message takeDmgMessage;
	takeDmgMessage.myMsg = eMessageType::ePlayerTookDMG;
	takeDmgMessage.aFloatValue = (float)myPlayerData.myCurrentHP / (float)myPlayerData.myMaxHP;
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
			if (myHealingItems > 0 && myPlayerData.myCurrentHP != myPlayerData.myMaxHP)
			{
				myHealing = true;
				myHealTimer = myHealingTime;
				myHealingItems--;
				myPlayerData.myCurrentHP++;

				INFO_PRINT("%s %i", "Player HP", myPlayerData);

				//Audio
				myAudioComponent->PlayEvent3D(FSPRO::Event::sfx_player_healing);

				//Send message to UI
				Message message;
				message.myMsg = eMessageType::ePlayerHealed;
				message.aFloatValue = (float)myPlayerData.myCurrentHP / (float)myPlayerData.myMaxHP;
				myPollingStation->myPostmaster->SendMsg(message);
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
	myPollingStation->myGameDataManager.get()->GetPlayerData().mySavePosition = myGameObject->GetTransform().GetPosition();
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
	/*auto playerAttack = playerRun->AddTransition("player_slash", [this]()->bool {return myAttack; });*/
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

	if (aOther->tag == eTag::health_pickup && myHealingItems < myMaxHealing)
	{
		myHealingItems++;
		myAudioComponent->PlayEvent3D(FSPRO::Event::sfx_player_interact);

		//HUD updates
		Message message;
		message.myMsg = eMessageType::ePlayerPickedUpHealth;
		myPollingStation->myPostmaster->SendMsg(message);

		myScene->RemoveGameObject(aOther);
		return;
	}

	if (aOther->tag == eTag::cassette_tape)
	{
		INFO_PRINT("casset tape");
		GameData& gameData = myPollingStation->myGameDataManager.get()->GetGameData();
		const int& number = aOther->GetComponent<CassetTapeComponent>()->GetNumber();
		gameData.myKeys[number] = true;
		
		myScene->RemoveGameObject(aOther);
		myAudioComponent->PlayEvent3D(FSPRO::Event::sfx_player_interact);

		//Load Hub if all keys are collected
		myPollingStation->mySceneManager.get()->LoadScene("Hub");
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

				INFO_PRINT("%s %i", "Player HP", myPlayerData.myCurrentHP);
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
			auto bulletComponent = aOther->GetComponent<BulletComponent>();
			if (bulletComponent != nullptr)
			{
				myTakeDamageTimer = myTakeDamageTime;
				std::cout << "player took damage by flute bullet\n";
				TakeDamage(bulletComponent->GetAttackDamage());
				bulletComponent->RemoveBullet();

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
}

void PlayerComponent::OnDeath()
{
	std::cout << "player died\n";
	GameData gameData = myPollingStation->myGameDataManager.get()->GetGameData();
	if (gameData.myCheckpoint != nullptr)
	{
		gameData.myCheckpoint->Load(); //causes crash
		myGameObject->GetTransform().SetPosition(myPlayerData.mySavePosition);
		return;
	}
	myPlayerData.mySavePosition = myPollingStation->myGameDataManager.get()->GetPlayerData().mySavePosition;
	myPlayerData.myCurrentHP = myPlayerData.myMaxHP;
	myGameObject->GetTransform().SetPosition(myPlayerData.mySavePosition);
}

void PlayerComponent::TakeDamage(int someDamage)
{
	myPlayerData.myCurrentHP -= someDamage;
	if (myPlayerData.myCurrentHP <= 0) { OnDeath(); }
	Message takeDmgMessage;
	takeDmgMessage.myMsg = eMessageType::ePlayerTookDMG;
	takeDmgMessage.aFloatValue = (float)myPlayerData.myCurrentHP / (float)myPlayerData.myMaxHP;
	myPollingStation->myPostmaster->SendMsg(takeDmgMessage);


	//chekded
}
#ifndef _RETAIL
void PlayerComponent::DebugUpdate()
{
	if (ImGui::CollapsingHeader("PlayerComponent"))
	{
		ImGui::InputInt("HP", &myPlayerData.myCurrentHP);
	}

}


#endif // _RETAIL


