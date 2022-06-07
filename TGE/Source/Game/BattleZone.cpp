#include "stdafx.h"
#include "BattleZone.h"
#include "GameObject.h"

BattleZone::BattleZone()
{

}

BattleZone::~BattleZone()
{
}

void BattleZone::OnCollisionStay(GameObject* /*anOther*/)
{
}

void BattleZone::OnAwake()
{
}

void BattleZone::OnStart()
{
}

void BattleZone::OnUpdate(const float aDeltaTime)
{
	Tga2D::Vector3f pos = myTransform->GetPosition();
	if (myGateIsOpen)
		return;

	if (myEnemyCounter > 0)
		return;

	if (myMoveTimer > 0)
	{
		myMoveTimer -= aDeltaTime;
		pos.y = pos.y - (10.f * aDeltaTime);
		myTransform->SetPosition(pos);
	}

	if (myMoveTimer <= 0)
		myGateIsOpen = true;
}
