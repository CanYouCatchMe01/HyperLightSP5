#pragma once
#include "InputObserver.h"
#include "InputMapperEvents.h"
#include "Component.h"
#include "Postmaster.h"
#include "fmod_studio.hpp"
#include <Tga2d/Math/Matrix.h>
#include "GameDataManager.h"
class PlayerComponent : public Component, Input::InputObserver
{
public:
	PlayerComponent(int aMaxHp, int aMaxHealing, int aMaxAttaks, float aDashTime, float aHealingtime, float aAttackTime, float aSpeed, float aDashSpeed);
	~PlayerComponent();
	void OnUpdate(float aDt) override;
	void Movement(float aDt);
	void Attack();

	void PickupHealing();

	Tga2D::Vector3f GetPosition();
	void SetPosition(Tga2D::Vector3f aPosition);

	int GetHp();
	void SetHp(int aHp);
	void SetFullHP();

	int GetHealing();
	void SetHealing(int aHealing);
	inline void SetGrounded(bool aIsGrounded) { myIsGrounded = aIsGrounded; };

	void RecieveEvent(const Input::eInputEvent aEvent, const float aValue) override;
	void SaveData();
	virtual void OnAwake() override;

	virtual void OnStart() override;
#ifndef _RETAIL
	void DebugUpdate() override;
#endif // _RETAIL

	void OnCollisionEnter(GameObject* aOther) override;
	void OnCollisionExit(GameObject* aOther) override;
	void OnEnemyHit();

	void OnDeath();
	void TakeDamage(int someDamage);


	PlayerData myPlayerData;
	bool myAttack = false;
private:
	Tga2D::Matrix4x4f myCameraRotation;
	float myRotation = 0;
	float myGoalRotation = 0;
	float myRotationDiff = 0;
	float myRotationTime = 0;
	float myRotationSpeed = 12; //Higher is faster.
	int myMaxHealing = 3;
	int myMaxAttacks = 3;
	float myDashTime = 0.1f;
	float myHealingTime = 0.1f;
	float myAttackTime = 1.f;
	float myDashCoolDown = 0.3f;
	float myGravity = 10.0f;
	float mySpeed = 0.5f;
	float myDashSpeed = 3.0f;
	float myTakeDamageTimer = 0.f;
	float myTakeDamageTime;

	Tga2D::Vector3f myDir = { 0.0f,0.0f,0.0f };
	Tga2D::Vector3f myLastDir = { 0.0f,0.0f,0.0f };
	Tga2D::Vector3f myDashDir = { 0.0f,0.0f,0.0f };

	float myDashTimer = 0.0f;
	float myAttackTimer = 0.0f;
	float myDashCollDownTimer = 0.0f;
	float myHealTimer = 0.0f;
	float myStunTimer = 0.0f;
	bool myDash = false;
	bool myStun = false;
	bool myIsGrounded = false;

	bool myHealing = false;
	bool myNextAttack = false;
	int myAttacks = 0;
	
	//Audio
	class AudioComponent* myAudioComponent = nullptr;

	//Save the walksound to increase and decrease the volume, when the player is moving
	FMOD::Studio::EventInstance* myWalkSound = nullptr;
	FMOD::Studio::EventInstance* myCheckpointSound = nullptr;
};

