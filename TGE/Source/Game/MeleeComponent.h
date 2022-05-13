#pragma once
#include "Component.h"
#include "BoxColliderComponent.h"

class MeleeComponent : public Component
{
public:
	MeleeComponent();
	~MeleeComponent();
	void OnUpdate(float aDeltaTime) override;
	int myDamage = 1;
	int myAttackSpeed = 1; // lower is faster.
#ifdef _DEBUG
	void DebugUpdate();
#endif // _DEBUG
	void OnAwake() override;

	void OnStart() override;
private:



};