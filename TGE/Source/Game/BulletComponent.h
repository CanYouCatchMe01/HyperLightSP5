#pragma once
#include "Component.h"

class BulletComponent : public Component
{
public:
	BulletComponent();
	BulletComponent(Tga2D::Vector3f aDirection, float aSpeed, int aDamedg);
	void OnUpdate(float aDT) override;
	void OnCollisionEnter(GameObject* aOther) override;
private:
	Tga2D::Vector3f myDir;
	float mySpeed;
	int myDamedg;
};

