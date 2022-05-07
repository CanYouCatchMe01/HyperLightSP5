#pragma once
#include "Component.h"

class EnemyComponent : public Component
{

public:
	EnemyComponent(int aType, int aMaxHp, float aSpeed, float anAttackSpeed, float aDetectionRadius);
	void OnUpdate(float aDT) override;
	
	void OnAwake();
	void OnStart();

	void SetPosition(Tga2D::Vector3f aPosition);
	Tga2D::Vector3f GetPosition();

private:
	void IdleMovement(float aDt);
	void MoveCloser(float aDT, Tga2D::Vector3f aDirection);
	void MoveAway(float aDT, Tga2D::Vector3f aDirection);
	void Charge(float aDT, Tga2D::Vector3f aDirection);
	void Attack(float aDT, Tga2D::Vector3f aDirection);

	Tga2D::Vector3f myAttackDirection;
	class GameObject* myTarget;
	int myType;
	int myHp;
	int myMaxHp;
	float mySpeed;
	float myAttackSpeed;
	float myDetectionRadius;
	float myTelegraph = 0.0f;
	float myAttackTime = 0.0f;
	bool myAttacking = false;
	bool myIsInRange = false;
};

