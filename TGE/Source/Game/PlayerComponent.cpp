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
#include "UpgradeComponent.h"
#include "GameDataManager.h"
#include "CollisionManager.h"

PlayerComponent::PlayerComponent(int aMaxHp, int aMaxHealing, int aMaxAttaks, float aDashTime, float aHealingtime, float aAttackTime, float aSpeed, float aDashSpeed, nlohmann::json aUpgradeMesh)
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
	myUpgradeMeshSettings = aUpgradeMesh;

	myDeathTimer.SetDuration(2.f);
	myDeathTimer.SetCallback([this]()
		{
			OnDeath();
		});
	
	mySetFullHpTimer.SetDuration(0.25f);
	mySetFullHpTimer.SetCallback([this]()
		{
			mySetFullHpTimer.Reset();
			myIsAlive = true;
		});
}

PlayerComponent::~PlayerComponent()
{
	GameData& gameData = myPollingStation->myGameDataManager.get()->GetGameData();
	gameData.myCheckpoint = nullptr; //Sets to nullptr else the game will crash, becuse it loads a new scene
}

void PlayerComponent::OnUpdate(float aDt)
{
	myTakeDamageTimer -= aDt;
	myDeathTimer.Update(aDt);
	mySetFullHpTimer.Update(aDt);

	if (!myIsGrounded && !myDash)
	{
		myDir.y = -myGravity / mySpeed;
		SetPosition(GetPosition() + (myDir * mySpeed * aDt));
	}

	if (!myStun && !myHealing /*&& !myAttack*/)
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
	myDashCollDownTimer -= aDt;
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

	//Just for LD to test
	if (myPollingStation->myInputMapper->GetInputManager().IsKeyPressed('I'))
	{
		myIsImmortal = true;
		INFO_PRINT("Immortal %i", myIsImmortal);
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
	myWalkSound->setVolume(1.0f);
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
	if (myRotationTime < 1 / myRotationSpeed && !myAttack)
	{
		myRotation += myRotationDiff * aDT * myRotationSpeed;
		myTransform->SetRotation({ 0,myRotation,0 });
		myRotationTime += aDT;
	}
	else if (myRotationTime < 1 / (myRotationSpeed / 5) && myAttack)
	{
		myRotation += myRotationDiff * aDT * (myRotationSpeed / 5);
		myTransform->SetRotation({ 0,myRotation,0 });
		myRotationTime += aDT;
	}
	if (myDash)
	{
		SetPosition(GetPosition() + myDashDir * myDashSpeed * aDT);
		return;
	}
	if (!myAttack)
	{
		SetPosition(GetPosition() + (myDir * mySpeed * aDT));
	}
	else
	{
		SetPosition(GetPosition() + (myDir * (mySpeed / 10) * aDT));
	}
}

void PlayerComponent::Attack()
{
	if (myAttackTimer < 0.0f)
	{
		myAttackTimer = myAttackTime;
		myAttack = true;
		//do the attack
		myAudioComponent->PlayEvent3D(FSPRO::Event::sfx_player_swing);
	}
}

void PlayerComponent::PickupHealing()
{
	if (myPlayerData.myHealKitAmmnt >= myMaxHealing)
	{
		myPlayerData.myHealKitAmmnt = myMaxHealing;
	}
	else
	{
		myPlayerData.myHealKitAmmnt++;
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

	SaveData();
}

void PlayerComponent::SetFullHP()
{
	myPlayerData.myCurrentHP = myPlayerData.myMaxHP;

	Message takeDmgMessage;
	takeDmgMessage.myMsg = eMessageType::ePlayerTookDMG;
	takeDmgMessage.aFloatValue = (float)myPlayerData.myCurrentHP / (float)myPlayerData.myMaxHP;
	myPollingStation->myPostmaster->SendMsg(takeDmgMessage);
	SaveData();
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
	if (!myIsAlive) { return; }
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
		if (myAttackTimer < 0.0f && !myHealing && !myAttack)
		{
			Attack();
		}
		break;
	case Input::eInputEvent::eDash:
	{
 		if (myDashTimer < 0 && myDashCollDownTimer < 0 && !myHealing && !myAttack)
		{
			myDashCollDownTimer = myDashCoolDown;
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
			if (myPlayerData.myHealKitAmmnt > 0 && myPlayerData.myCurrentHP != myPlayerData.myMaxHP)
			{
				myHealing = true;
				myHealTimer = myHealingTime;
				myPlayerData.myHealKitAmmnt--;
				myPlayerData.myCurrentHP++;

				INFO_PRINT("%s %i", "Player HP", myPlayerData);

				//Audio
				myAudioComponent->PlayEvent3D(FSPRO::Event::sfx_player_healing);

				//Send message to UI
				Message message;
				message.myMsg = eMessageType::ePlayerHealed;
				message.aFloatValue = (float)myPlayerData.myCurrentHP / (float)myPlayerData.myMaxHP;
				myPollingStation->myPostmaster->SendMsg(message);

				PlayerData& tempData = myPollingStation->myGameDataManager.get()->GetPlayerData();
				tempData.myCurrentHP = myPlayerData.myCurrentHP;
			}
			else
			{
				//cry
			}
		}
		break;
	}
}

void PlayerComponent::SaveData()
{
	PlayerData& playerData = myPollingStation->myGameDataManager.get()->GetPlayerData();
	playerData = myPlayerData;
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
}

void PlayerComponent::OnStart()
{
	GameData& gamedata = myPollingStation->myGameDataManager.get()->GetGameData();
	if (gamedata.myUpgrades[1])
		SetUpgradeMesh();

	SetPollingStation(myPollingStation);
	std::string sceneName = myPollingStation->mySceneManager.get()->GetActiveScene()->name;
	if (sceneName == "Jungle 1" || sceneName == "Jungle 2" || sceneName == "Jungle 3")
	{
		myAmbience = myAudioComponent->PlayEvent3D(FSPRO::Event::sfx_world_ambient_jungle);
		myWalkSound = myAudioComponent->PlayEvent3D(FSPRO::Event::sfx_player_walk_jungle);
	}
	else if (sceneName == "Badlands 1" || sceneName == "Badlands 2" || sceneName == "Badlands 3")
	{
		myWalkSound = myAudioComponent->PlayEvent3D(FSPRO::Event::sfx_player_walk_badlands);
	}
	else
	{
		myWalkSound = myAudioComponent->PlayEvent3D(FSPRO::Event::sfx_player_walk_hub);
	}


	myWalkSound->setVolume(0.0f);
	myTakeDamageTime = 1.f;
	auto animatedMesh = myGameObject->GetComponent<AnimatedMeshComponent>();
	ApplyAnimations(animatedMesh);

	PlayerData tempData = myPollingStation->myGameDataManager.get()->GetPlayerData();
	if (tempData.myCurrentHP > 0)
	{
		myPlayerData = tempData;
	}
}

void PlayerComponent::OnCollisionEnter(GameObject* aOther)
{
	if (aOther->tag == eTag::teleporter)
	{
		SaveData();
		aOther->GetComponent<TeleporterComponent>()->Load();
		myAudioComponent->PlayEvent3D(FSPRO::Event::sfx_player_death);
		return;
	}

	if (aOther->tag == eTag::health_pickup && myPlayerData.myHealKitAmmnt < myMaxHealing)
	{
		myPlayerData.myHealKitAmmnt++;
		myAudioComponent->PlayEvent3D(FSPRO::Event::sfx_player_healing);

		////update gameData
		//PlayerData& playerData = myPollingStation->myGameDataManager.get()->GetPlayerData();
		//playerData.myHealKitAmmnt = myHealingItems;

		//HUD updates
		Message message;
		message.myMsg = eMessageType::ePlayerPickedUpHealth;
		myPollingStation->myPostmaster->SendMsg(message);

		myScene->RemoveGameObject(aOther);
		SaveData();
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
		myPollingStation->mySceneManager.get()->LoadScene("Hub", "HubTeleport");

		Message cassetPicked;
		cassetPicked.myMsg = eMessageType::ePickUpKey;
		cassetPicked.anIntValue = number;
		myPollingStation->myPostmaster.get()->SendMsg(cassetPicked);
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

	if (myIsAlive && aOther->tag == eTag::bullet)
	{
		auto bulletComponent = aOther->GetComponent<BulletComponent>();
		if (bulletComponent != nullptr)
		{
			myTakeDamageTimer = myTakeDamageTime;
			std::cout << "player took damage by flute bullet\n";
			TakeDamage(bulletComponent->GetAttackDamage());
			bulletComponent->RemoveBullet();

			//Hit audio
			myAudioComponent->PlayEvent3D(FSPRO::Event::sfx_enemy_attack_worm_hit);
		}
	}

	if (aOther->tag == eTag::upgrade)
	{
		int upgradeIndex = aOther->GetComponent<UpgradeComponent>()->GetIndex();
		Message upgradeMsg;
		if (upgradeIndex == 0)
		{
			upgradeMsg.myMsg = eMessageType::eHealthUpgrade;
		}
		else
		{
			upgradeMsg.myMsg = eMessageType::eWeaponUpgrade;
			SetUpgradeMesh();

		}
		myPollingStation->myPostmaster.get()->SendMsg(upgradeMsg);
		GameData& gamedata = myPollingStation->myGameDataManager.get()->GetGameData();
		gamedata.myUpgrades[upgradeIndex] = true;
		myScene->RemoveGameObject(aOther);
	}
}

void PlayerComponent::OnCollisionStay(GameObject* aOther)
{
	if (myTakeDamageTimer < 0.f && myIsAlive)
	{
		if (aOther->tag == eTag::popcorn)
		{
			if (aOther->GetComponent<PopcornEnemy>() != nullptr)
			{
				myTakeDamageTimer = myTakeDamageTime;
				std::cout << "player took damage by popcorn\n";
				TakeDamage(aOther->GetComponent<PopcornEnemy>()->GetAttackDmg());
				aOther->GetComponent<PopcornEnemy>()->myIsStunned = true;

				//Worm is hitting
				myAudioComponent->PlayEvent3D(FSPRO::Event::sfx_enemy_attack_worm_hit);

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

				//Guitar hit
				myAudioComponent->PlayEvent3D(FSPRO::Event::sfx_enemy_attack_guitar_hit);
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

void PlayerComponent::OnEnemyHit()
{
	myAudioComponent->PlayEvent3D(FSPRO::Event::sfx_player_slash);
}

void PlayerComponent::OnDeath()
{
	myIsAlive = false;
	myAudioComponent->PlayEvent3D(FSPRO::Event::sfx_player_death);
	myDeathTimer.Stop();
	myDeathTimer.Reset();
	GameData& gameData = myPollingStation->myGameDataManager.get()->GetGameData();
	if (gameData.myCheckpoint != nullptr)
	{
		mySetFullHpTimer.Start();
		SetFullHP();
		gameData.myCheckpoint->Load(); //causes crash
		//myGameObject->GetTransform().SetPosition(myPlayerData.mySavePosition);
		return;
	}
	SetFullHP();
	myPlayerData = myPollingStation->myGameDataManager.get()->GetPlayerData();
//	myPlayerData.myCurrentHP = myPlayerData.myMaxHP;
	mySetFullHpTimer.Start();
	myGameObject->GetTransform().SetPosition(myPlayerData.mySavePosition);
}

void PlayerComponent::TakeDamage(int someDamage)
{
	//Just for LD to test
	if (myIsImmortal)
	{
		return;
	}
	
	myPlayerData.myCurrentHP -= someDamage;
	if (myPlayerData.myCurrentHP <= 0) { myIsAlive = false; myDeathTimer.Start(); }
	else { myDeathTimer.Stop(); }
	Message takeDmgMessage;
	takeDmgMessage.myMsg = eMessageType::ePlayerTookDMG;
	takeDmgMessage.aFloatValue = (float)myPlayerData.myCurrentHP / (float)myPlayerData.myMaxHP;
	myPollingStation->myPostmaster->SendMsg(takeDmgMessage);

	SaveData();
	//chekded
}

void PlayerComponent::SetUpgradeMesh()
{
	myGameObject->RemoveComponent<AnimatedMeshComponent>();

	auto data = myUpgradeMeshSettings["components"][0]["data"];

	auto mesh = myGameObject->AddComponent<AnimatedMeshComponent>(data["model"], data["albedo"], data["normal"], data["reflective"], data["animations"], myScene, myGameObject);
	ApplyAnimations(mesh);
	

}

void PlayerComponent::ApplyAnimations(AnimatedMeshComponent* aMesh)
{
	auto playerIdle = aMesh->AddTransition("Player_Idle", [this]()->bool { return true; });
	auto playerRun = playerIdle->AddTransition("Player_Run", [this]()->bool { return (myLastDir.x != 0 || myLastDir.z != 0); });
	auto playerDash = playerRun->AddTransition("Player_Dash", [this]()->bool { return myDash; });
	auto playerAttack = aMesh->AddTransition("Player_Slash", [this]()->bool { return myAttack; });
	auto playerDeath = aMesh->AddTransition("Player_Death", [this]()->bool { return myPlayerData.myCurrentHP <= 0; });
	playerRun->AddTransition(playerAttack, [this]()->bool { return myAttack; });
	playerIdle->AddTransition(playerAttack, [this]()->bool { return myAttack; });
	playerAttack->AddTransition(playerIdle, [this]()->bool { return (!myAttack && (myLastDir.x == 0 && myLastDir.z == 0)); });
	playerAttack->AddTransition(playerRun, [this]()->bool { return (!myAttack && (myLastDir.x != 0 || myLastDir.z != 0)); });
	playerRun->AddTransition(playerIdle, [this]()->bool { return (myLastDir.x == 0 && myLastDir.z == 0); });
	playerDash->AddTransition(playerIdle, [this]()->bool { return (myLastDir.x == 0 && myLastDir.z == 0 && !myDash); });
	playerDash->AddTransition(playerRun, [this]()->bool { return ((myLastDir.x != 0 || myLastDir.z != 0) && !myDash); });
	playerRun->AddTransition(playerDeath, [this]()->bool { return myPlayerData.myCurrentHP <= 0; });
	playerIdle->AddTransition(playerDeath, [this]()->bool { return myPlayerData.myCurrentHP <= 0; });
	playerAttack->AddTransition(playerDeath, [this]()->bool { return myPlayerData.myCurrentHP <= 0; });
	playerDash->AddTransition(playerDeath, [this]()->bool { return myPlayerData.myCurrentHP <= 0; });
	playerDeath->AddTransition(playerIdle, [this]()->bool { return myPlayerData.myCurrentHP > 0; });
	playerDeath->AddTransition(playerRun, [this]()->bool { return myPlayerData.myCurrentHP > 0; });
	playerDeath->AddTransition(playerAttack, [this]()->bool { return myPlayerData.myCurrentHP > 0; });
	playerDeath->AddTransition(playerDash, [this]()->bool { return myPlayerData.myCurrentHP > 0; });
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
