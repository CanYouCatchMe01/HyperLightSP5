#include "stdafx.h"
#include "PlayerComponent.h"
#include <iostream>
#include "GameObject.h"

PlayerComponent::PlayerComponent()
{
}

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
	if (!myStun)
	{
		if (!myHealing && !myAttack)
		{
			Movement(aDT);
		}

		if (!myDash && !myHealing)
		{
			Attack(aDT);
		}

	}
	myStunTimer -= aDT;
	if (myStunTimer < 0.0f)
	{
		myStun = false;
	}
	myDashTimer -= aDT;
	if (myDashTimer < 0)
	{
		myDash = false;
	}
	myAttackTimer -= aDT;
	if (myAttackTimer < 0.05f)
	{
		myAttack = false;
	}
	if (myAttackTimer < -0.05f)
	{
		myAttacks = 0;
	}
	myHealTimer -= aDT;
	if (myHealTimer < 0.0f)
	{
		myHealing = false;
	}
}

void PlayerComponent::Movement(float aDT)
{
	//myDir.Normalize();
	//if (myDir.x != 0 || myDir.y != 0)
	//{
	//	myLastDir = myDir;
	//}

	//if (myDash)
	//{
	//	myTransform->SetPosition(myTransform->GetPosition() + myDashDir * myDashSpeed * aDT);
	//}

	//if (!myDash)
	//{
	//	myTransform->SetPosition(myTransform->GetPosition() + myDir * mySpeed * aDT);
	//}
	//myDir = { 0.0f,0.0f,0.0f };

	// right
	if (GetAsyncKeyState(0x44))
	{
		myDir = myTransform->GetMatrix().GetRight();
	}
	// left
	if (GetAsyncKeyState(0x41))
	{
		myDir = myTransform->GetMatrix().GetRight() * -1.f;
	}
	// forward
	if (GetAsyncKeyState(0x57))
	{
		myDir = myTransform->GetMatrix().GetForward();
	}
	// backward
	if (GetAsyncKeyState(0x53))
	{
		myDir = myTransform->GetMatrix().GetForward() * -1.f;
	}

	// up right
	if (GetAsyncKeyState(0x44) && GetAsyncKeyState(0x57))
	{
		Tga2D::Vector3f topRight = myTransform->GetMatrix().GetRight() + myTransform->GetMatrix().GetForward();
		topRight.Normalize();
		myDir = topRight;
	}
	// up left
	if (GetAsyncKeyState(0x41) && GetAsyncKeyState(0x57))
	{
		Tga2D::Vector3f topLeft = (myTransform->GetMatrix().GetRight() * -1.f) + myTransform->GetMatrix().GetForward();
		topLeft.Normalize();
		myDir = topLeft;
	}
	// down right
	if (GetAsyncKeyState(0x44) && GetAsyncKeyState(0x53))
	{
		Tga2D::Vector3f downRight = myTransform->GetMatrix().GetRight() + (myTransform->GetMatrix().GetForward() * -1.f);
		downRight.Normalize();
		myDir = downRight;
	}
	// down left
	if (GetAsyncKeyState(0x41) && GetAsyncKeyState(0x53))
	{
		Tga2D::Vector3f downLeft = (myTransform->GetMatrix().GetRight() * -1.f) + (myTransform->GetMatrix().GetForward() * -1.f);
		downLeft.Normalize();
		myDir = downLeft;
	}

	
	myDir.Normalize();
	if (myDir.x != 0 || myDir.z != 0)
	{
		myLastDir = myDir;
	}

	if (myDash)
	{
		SetPosition(GetPosition() + myDashDir * myDashSpeed * aDT);
	}
	else
	{
		if (GetAsyncKeyState(0x20) && myDashTimer < -0.1)
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
	}

	if (!myDash)
	{
		myDir.y -= myGravity / mySpeed;
		SetPosition(GetPosition() + myDir * mySpeed * aDT);
	}
	myDir = { 0.0f,0.0f,0.0f };
}

void PlayerComponent::Attack(float /*aDT*/)
{	
	if (myNextAttack)
	{
		if (myAttackTimer < 0.0f)
		{
			myAttackTimer = 0.1f;
			myAttack = true;
		}
	}
	else if (myAttack)
	{
		//do the attack
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
		myDir.y += 1.0f;
		break;
	case Input::eInputEvent::eMoveUp:
		myDir.y -= 1.0f;
		break;
	case Input::eInputEvent::eMoveRight:
		myDir.x += 1.0f;
		break;
	case Input::eInputEvent::eMoveLeft:
		myDir.x -= 1.0f;
		break;
	case Input::eInputEvent::eAttack:
		if (myAttacks < myMaxAttacks)
		{
			if (myAttackTimer < 0.0f)
			{
				myAttackTimer = myAttackTime;
				myAttack = true;
				myAttacks++;
			}
			else if (!myNextAttack)
			{
				myNextAttack = true;
				myAttacks++;
			}
		}
		break;
	case Input::eInputEvent::eDash:
		if (myDashTimer < -0.1)
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
				//no healing
			}
		}
		break;
	}
}


void PlayerComponent::OnAwake()
{
}
void PlayerComponent::OnStart()
{
	myPollingStation->myPlayer = myGameObject;
}