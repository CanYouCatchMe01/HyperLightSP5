#include "stdafx.h"
#include "BulletComponent.h"
#include "GameObject.h"
#include "PlayerComponent.h"

BulletComponent::BulletComponent()
{
	myDir = { 0.0f,0.0f,0.0f };
	mySpeed = 0.0f;
	myDamedg = 0;
}

BulletComponent::BulletComponent(Tga2D::Vector3f aDirection, float aSpeed, int aDamedg)
{
	myDir = aDirection;
	mySpeed = aSpeed;
	myDamedg = aDamedg;
}

void BulletComponent::OnUpdate(float aDT)
{
	myTransform->SetPosition(myTransform->GetPosition() + myDir * mySpeed * aDT);
}

void BulletComponent::OnCollisionEnter(GameObject* aOther)
{
	PlayerComponent* a = aOther->GetComponent<PlayerComponent>();
	if (a != nullptr)
	{
		a->TakeDamedg(myDamedg);
		//destroy bullet
	}
}
