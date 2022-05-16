#pragma once
#include "InputObserver.h"
#include "InputMapperEvents.h"
#include "Component.h"
#include "Postmaster.h"
#include "fmod_studio.hpp"


class PlayerComponent : public Component, Input::InputObserver
{
public:
	PlayerComponent(int aMaxHp, int aMaxHealing, int aMaxAttaks, float aDashTime, float aHealingtime, float aAttackTime, float aSpeed, float aDashSpeed);
	void OnUpdate(float aDT) override;
	void Movement(float aDT);
	void Attack();

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
#ifdef _DEBUG
	void DebugUpdate() override;
#endif // _DEBUG

	void OnCollisionEnter(GameObject* aOther) override;
	void OnCollisionExit(GameObject* aOther) override;

	void OnDeath();
	void TakeDamage(int someDamage);



	bool myAttack = false;
private:
	int myMaxHp = 5;
	int myMaxHealing = 3;
	int myMaxAttacks = 3;
	float myDashTime = 0.1f;
	float myHealingTime = 0.1f;
	float myAttackTime = 0.1f;
	float myGravity = 10.0f;
	float mySpeed = 0.5f;
	float myDashSpeed = 3.0f;

	Tga2D::Vector3f myDir = { 0.0f,0.0f,0.0f };
	Tga2D::Vector3f myLastDir = { 0.0f,0.0f,0.0f };
	Tga2D::Vector3f myDashDir = { 0.0f,0.0f,0.0f };
	float myDashTimer = 0.0f;
	float myAttackTimer = 0.0f;
	float myHealTimer = 0.0f;
	float myStunTimer = 0.0f;
	bool myDash = false;
	bool myStun = false;

	bool myHealing = false;
	bool myNextAttack = false;
	int myAttacks = 0;
	int myHp = myMaxHp;
	int myHealingItems = 0;
	
	//Audio
	class AudioComponent* myAudioComponent = nullptr;

	//Save the walksound to increase and decrease the volume, when the player is moving
	FMOD::Studio::EventInstance* myWalkSound = nullptr;
};

