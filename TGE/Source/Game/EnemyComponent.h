#pragma once
#include "Component.h"

class GameObject;

class EnemyComponent : public Component
{
public:
	EnemyComponent();
	virtual ~EnemyComponent() = default;

	void OnUpdate(float aDt) override;
	void OnAwake();
	void OnStart();

	virtual void Attack(float aDT, Tga2D::Vector3f aDirection) = 0;
	
protected:
	// to do, complete the IdleMovement function

	virtual void CheckRadius();

	virtual void SetPosition(const Tga2D::Vector3f& aPosition);
	virtual Tga2D::Vector3f GetPosition();
	virtual void IdleMovement(float aDt);
	virtual void MoveTowardsPlayer(float aDT);

	Tga2D::Vector3f myDistanceToTarget;
	GameObject* myTarget;
	bool myIsInRange = false;

	int myMaxHp;
	int myAttackDmg;
	int myRandNum;

	float myMoveTimer = 0.0f;
	float myMoveTime;

	float mySpeed;
	float myAttackSpeed;
	float myDetectionRadius;
private:

};