#pragma once
#include "Component.h"

class BattleZone : public Component
{
public:
	BattleZone();
	~BattleZone();
	void OnCollisionStay(GameObject* anOther) override;
	void OnAwake() override;
	void OnStart() override;
	void OnUpdate(const float aDeltaTime) override;
private:
	bool myHasEnemies =  false;
	bool myGateIsOpen = false;
	float myMoveTimer = 1.f;
};