#include "stdafx.h"
#include "BulletComponent.h"
#include "Scene.h"

BulletComponent::BulletComponent(float aSpeed, int someDamage)
{
	mySpeed = aSpeed;
	myAttackDamage = someDamage;
}

void BulletComponent::OnAwake()
{
}

void BulletComponent::OnStart()
{
}

void BulletComponent::OnUpdate(float aDt)
{
	myTransform->SetPosition(myTransform->GetPosition() + myAttackDirection * mySpeed * aDt);
}

void BulletComponent::SetDirection(Tga2D::Vector3f aDirection)
{
	myAttackDirection = aDirection;
}

void BulletComponent::SetPosition(Tga2D::Vector3f aPosition)
{
	myTransform->SetPosition(aPosition);
}

void BulletComponent::RemoveBullet()
{
	myScene->RemoveGameObject(myGameObject);
}

int BulletComponent::GetAttackDamage()
{
	return myAttackDamage;
}
