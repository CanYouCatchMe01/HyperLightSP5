#include "stdafx.h"
#include "PlayerComponent.h"
#include <iostream>
#include "GameObject.h"
#include "EnemyComponent.h"
#include "AudioComponent.h"

PlayerComponent::PlayerComponent(int aMaxHp, int aMaxHealing, int aMaxAttaks, float aDashTime, float aHealingtime, float aAttackTime, float aSpeed, float aDashSpeed)
{
	myMaxHp = aMaxHp;
	myHp = myMaxHp;
	myMaxHealing = aMaxHealing;
	myMaxAttacks = aMaxAttaks;
	myDashTime = aDashTime;
	myHealingTime = aHealingtime;
	myAttackTime = aAttackTime;
	mySpeed = aSpeed;
	myDashSpeed = aDashSpeed;
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
	}
	else
	{
	}

	if (myDir.Length() > 0)
	{
		myDir.Normalize();
	}
	
	
	if (myDir.x != 0 || myDir.z != 0)
	{
		myLastDir = myDir;
	}
	if (myDash)
	{
		SetPosition(GetPosition() + myDashDir * myDashSpeed * aDT);
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
		myAudioComponent->PlayEvent(FSPRO::Event::sfx_player_attack);
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
		myDir.z -= 1.0f;
		break;
	case Input::eInputEvent::eMoveUp:
		myDir.z += 1.0f;
		break;
	case Input::eInputEvent::eMoveRight:
		myDir.x += 1.0f;
		break;
	case Input::eInputEvent::eMoveLeft:
		myDir.x -= 1.0f;
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
}
void PlayerComponent::OnCollisionEnter(GameObject* aOther)
{
	EnemyComponent* enemy = aOther->GetComponent<EnemyComponent>();

	if (enemy != nullptr)
	{
		TakeDamage(enemy->GetAttackDmg());
	}
}

void PlayerComponent::OnDeath()
{
	// you ded, now what?
}

void PlayerComponent::TakeDamage(int someDamage)
{
	myHp -= someDamage;
	/*SoundEngine::PlayEvent(takeDamageSound?);*/
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


