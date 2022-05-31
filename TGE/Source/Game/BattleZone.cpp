#include "stdafx.h"
#include "BattleZone.h"
#include "GameObject.h"

BattleZone::BattleZone()
{

}

BattleZone::~BattleZone()
{
}

void BattleZone::OnCollisionStay(GameObject* anOther)
{
	if (anOther->tag == eTag::flute || anOther->tag==eTag::charge || anOther->tag == eTag::popcorn)
	{
		myHasEnemies = true;
		return;
	}
	else
		myHasEnemies = false;
}

void BattleZone::OnAwake()
{}

void BattleZone::OnStart()
{}

void BattleZone::OnUpdate(const float aDeltaTime)
{
	if (myHasEnemies)
		return;

	if (myGateIsOpen)
		return;

	if (myMoveTimer > 0)
	{
		myMoveTimer -= aDeltaTime;
		Tga2D::Vector3f pos = myTransform->GetPosition();
		pos.y -= 5.f * aDeltaTime;
		myTransform->SetPosition(pos);
		return;
	}
	myGateIsOpen = true;
}
