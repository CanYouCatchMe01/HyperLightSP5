#pragma once
#include "InputObserver.h"
#include "InputMapperEvents.h"
#include "Component.h"

class PlayerComponent : public Component, Input::InputObserver
{
public:
	PlayerComponent();
	PlayerComponent(int aMaxHp, int aMaxHealing, int aMaxAttaks, float aDashTime, float aHealingtime, float aAttackTime, float aSpeed, float aDashSpeed);
	void OnUpdate(float aDT) override;
	void Movement(float aDT);
	void Attack(float aDT);

	void PickupHealing();

	Tga2D::Vector3f GetPosition();
	void SetPosition(Tga2D::Vector3f aPosition);

	int GetHp();
	void SetHp(int aHp);

	int GetHealing();
	void SetHealing(int aHealing);

	void RecieveEvent(const Input::eInputEvent aEvent, const float aValue) override;

	virtual void OnAwake() override;

	virtual void OnStart() override;

private:
	int myMaxHp = 5;
	int myMaxHealing = 3;
	float myDashTime = 0.1f;
	float myHealingTime = 0.1f;
	float myAttackTime = 0.1f;
	float myGravity = 10.0f;
	int myMaxAttacks = 3;


	Tga2D::Vector3f myDir = { 0.0f,0.0f,0.0f };
	Tga2D::Vector3f myLastDir = { 0.0f,0.0f,0.0f };
	Tga2D::Vector3f myDashDir = { 0.0f,0.0f,0.0f };
	float mySpeed = 0.5f;
	float myDashSpeed = 3.0f;
	float myDashTimer = 0.0f;
	float myAttackTimer = 0.0f;
	float myHealTimer = 0.0f;
	float myStunTimer = 0.0f;
	bool myDash = false;
	bool myStun = false;
	bool myAttack = false;
	bool myHealing = false;
	bool myNextAttack = false;
	int myAttacks = 0;
	int myHp = myMaxHp;
	int myHealingItems = 0;
};

