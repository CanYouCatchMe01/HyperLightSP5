#pragma once
#include "Component.h"

class BattleZone : public Component
{
public:
	BattleZone();
	~BattleZone();
	inline void AddEnemy() { myEnemyCounter++; }
	inline void RemoveEnemy() { myEnemyCounter--; }
	void OnCollisionStay(GameObject* anOther) override;
	void OnAwake() override;
	void OnStart() override;
	void OnUpdate(const float aDeltaTime) override;
private:
	float myActivationTimer = 0.5f;
	int myEnemyCounter = 0;
	bool myGateIsOpen = false;
	float myMoveTimer = 1.f;

	//Audio
	class AudioComponent* myAudioComponent = nullptr;
};