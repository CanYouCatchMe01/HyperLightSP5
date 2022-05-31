#pragma once
#include "Component.h"
#include "fmod_studio.hpp"

class EnemyComponent : public Component
{
public:
	EnemyComponent();
	virtual ~EnemyComponent() = default;

	void OnUpdate(float aDt) override;
	void OnAwake() override;
	void OnStart() override;

	void OnCollisionStay(GameObject* aOther);
	virtual void TakeDamage(int someDamage);
	virtual int GetAttackDmg();

	virtual void OnDeath() = 0;

protected:
	virtual void CheckRadius();
	void CorrectRotation(float aDeltaTime);

	virtual void SetPosition(const Tga2D::Vector3f& aPosition);
	virtual Tga2D::Vector3f GetPosition();

	virtual void IdleMovement(float aDt);
	virtual void MoveTowardsPlayer(float aDt);

	Tga2D::Vector3f myStartPosition;
	Tga2D::Vector3f myDistanceToTarget;
	class GameObject* myTarget;
	bool myIsInRange = false;
	bool myIsDead = false;
	bool myHasTurned = false;

	int myMaxHp;
	int myAttackDmg;
	int myRandNum;
	
	float myIdleRadius;
	float myTakeDamageTimer = 0.f;
	float myTakeDamageTime;
	float myGravity = 5.5f;
	float mySpeed;
	float myIdleSpeed;
	float myAttackSpeed;
	float myDetectionRadius;
	float myMoveTimer = 0.f;
	float myMoveTime;
	float myRotation = 0;
	float myGoalRotation = 0;
	float myRotationDiff = 0;
	float myRotationTime = 0;
	float myRotationSpeed = 12; //Higher is faster.

	//Audio
	class AudioComponent* myAudioComponent = nullptr;

	//Save the walksound to increase and decrease the volume, when the player is moving
	FMOD::Studio::EventInstance* myWalkSound = nullptr;
public:
	Tga2D::Vector3f myDir;
	Tga2D::Vector3f myLastDir;
	bool myIsStunned = false;
};