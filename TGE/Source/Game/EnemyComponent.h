#pragma once
#include "Component.h"

class EnemyComponent : public Component
{
public:
	EnemyComponent();
	virtual ~EnemyComponent() = default;

	void OnUpdate(float aDt) override;
	void OnAwake() override;
	void OnStart() override;

	virtual void OnCollisionEnter(GameObject* aOther);
	virtual void TakeDamage(int someDamage);
	virtual int GetAttackDmg();

	virtual void Attack(float aDT, Tga2D::Vector3f aDirection) = 0;
	virtual void OnDeath() = 0;

protected:
	virtual void CheckRadius();

	virtual void SetPosition(const Tga2D::Vector3f& aPosition);
	virtual Tga2D::Vector3f GetPosition();
	

	virtual void IdleMovement(float aDt);
	virtual void MoveTowardsPlayer(float aDT);

	Tga2D::Vector3f myDistanceToTarget;
	class GameObject* myTarget;
	bool myIsInRange = false;
	bool myIsDead = false;

	int myMaxHp;
	int myHp;
	int myAttackDmg;
	int myRandNum;

	float myGravity = 5.5f;
	float mySpeed;
	float myIdleSpeed;
	float myAttackSpeed;
	float myDetectionRadius;
	float myMoveTimer = 0.0f;
	float myMoveTime;

public:
	bool myIsStunned = false;
};