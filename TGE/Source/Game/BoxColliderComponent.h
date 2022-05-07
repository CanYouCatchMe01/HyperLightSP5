#pragma once
#include "Component.h"

class BoxColliderComponent : public Component
{
public:
	BoxColliderComponent(Tga2D::Vector3f aSize, Tga2D::Vector3f aCenter, bool aIsStatic, bool aIsTrigger);
	~BoxColliderComponent();
	// Inherited via Component
	virtual void OnAwake() override;

	virtual void OnStart() override;

	virtual void OnUpdate(const float aDeltaTime) override;

private:
	bool myIsStatic;
	bool myIsTrigger;
	size_t myHandle;
	Tga2D::Vector3f myCenter;
	Tga2D::Vector3f mySize;
};