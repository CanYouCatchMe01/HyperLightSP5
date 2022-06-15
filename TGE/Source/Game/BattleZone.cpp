#include "stdafx.h"
#include "BattleZone.h"
#include "GameObject.h"
#include "AudioComponent.h"

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
	myAudioComponent = myGameObject->AddComponent<AudioComponent>();
}

void BattleZone::OnStart()
{
}

void BattleZone::OnUpdate(const float aDeltaTime)
{
	if (myActivationTimer > 0)
	{
		myActivationTimer -= aDeltaTime;
		return;
	}

	Tga2D::Vector3f pos = myTransform->GetPosition();
	if (myGateIsOpen)
		return;

	if (myEnemyCounter > 0)
		return;

	if (myMoveTimer == 1.0f) //Play audio one, myMoveTimer will decrease later
	{
		myAudioComponent->PlayEvent3D(FSPRO::Event::sfx_player_death);
	}

	if (myMoveTimer > 0)
	{
		myMoveTimer -= aDeltaTime;
		pos.y = pos.y - (10.f * aDeltaTime);
		myTransform->SetPosition(pos);
	}

	if (myMoveTimer <= 0)
		myGateIsOpen = true;
}
