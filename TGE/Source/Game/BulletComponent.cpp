#include "stdafx.h"
#include "BulletComponent.h"

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
