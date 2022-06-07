#pragma once
#include <vector>
#include <tga2d/math/Vector3.h>
#include <tga2d/math/Matrix4x4.h>
#include <tga2d/math/Transform.h>
#include "GameObject.h"
#include <array>
#include "HashMap.hpp"

using SetOfNormals = std::array<Tga2D::Vector3f, 6>;
using SetOfCorners = std::array<Tga2D::Vector3f, 8>;

using Vector3 = Tga2D::Vector3f;

class OBB3D
{
public:
	friend class BoxColliderComponent;
	friend class CollisionManager;

	OBB3D() = default;
	OBB3D(Vector3 aSize, Vector3 anOffset, bool aIsStatic = false, bool aIsTrigger = false, GameObject* aParent = nullptr);
	bool Collides(OBB3D& aOther);
	void Calculate();

public:
	void SetTransform(Tga2D::Transform& aTransform);
#ifdef _DEBUG
	void Draw();
#endif
private:
	void CallGrounded();
	void SetGrounded(bool aState);
	void SetPosition(Tga2D::Vector3f aPos);
	void SATTest(const Vector3 anAxis, const SetOfCorners& aPtSet, float& aMinExtent, float& aMaxExtent);
	bool Overlaps(float aMin1, float aMax1, float aMin2, float aMax2); 
	void SetCollisionEvent(bool aCollided, OBB3D& aOther);
	bool GetMTVTranslation(OBB3D& aOtherObb, Vector3& aMtv, float& aMinTranslation, Vector3& aMaxTv, float& aMaxTranslation);
	Vector3 AveragePos();

	enum class eCollisionState
	{
		eNone,
		eEnter,
		eStay,
		eExit
	};

	enum class GroundState
	{
		eNone,
		eAirborne,
		eGrounded,
	};

	bool myDrawHitbox = false;
	bool myAlwaysSendEvent = false;
	bool myIsStatic;
	bool myIsTrigger;

	bool myIsGrounded = false;
	bool myCalledGroundedThisUpdate = false;

	Vector3 myOffset;
	Vector3 mySize;

	float myMaxStepHeight = 0.2f;
	float myMaxRadiusSqr;
	
	GameObject* myParent;
	
	SetOfNormals myNormals;
	SetOfCorners myCorners;

	Tga2D::Transform myTransform;
	CommonUtilities::HashMap<GameObject*, eCollisionState> myCurrentlyColliding;
	//std::unordered_map<GameObject*, eCollisionState> myCurrentlyColliding;
};