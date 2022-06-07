#pragma once
#include "Component.h"

class BulletComponent : public Component
{
public:
	BulletComponent(float aSpeed, int someDamage);
	void OnAwake() override;
	void OnStart() override;
	void OnUpdate(float aDt) override;
	void SetDirection(Tga2D::Vector3f aDirection);
	void SetPosition(Tga2D::Vector3f aPosition);
	void RemoveBullet();
	int GetAttackDamage();
private:
	Tga2D::Vector3f myAttackDirection;
	float mySpeed;
	int myAttackDamage;
};

