#pragma once
#include <vector>
#include <tga2d/math/Vector3.h>
#include <tga2d/math/Matrix4x4.h>
#include <tga2d/math/Transform.h>
#include "GameObject.h"

using SetOfNormals = std::vector<Tga2D::Vector3f>;
using SetOfCorners = std::vector<Tga2D::Vector3f>;

using Vector3 = Tga2D::Vector3f;

class OBB3D
{
public:
	OBB3D(Vector3 aSize, Vector3 anOffset, bool aIsStatic = false, bool aIsTrigger = false, GameObject* aParent = nullptr);
	bool Collides(OBB3D& aOther);
	void Calculate();

public:
	void SetTransform(Tga2D::Transform& aTransform);
	void Draw();
private:
	void SATTest(const Vector3 anAxis, const SetOfCorners& aPtSet, float& aMinExtent, float& aMaxExtent);
	bool Overlaps(float aMin1, float aMax1, float aMin2, float aMax2); 
	void SetCollisionEvent(bool aCollided, OBB3D& aOther);
	bool GetMTVTranslation(OBB3D& aOtherObb, Vector3& aMtv, float& aMinTranslation, Vector3& aMaxTv, float& aMaxTranslation);

	enum class eCollisionState
	{
		eNone,
		eEnter,
		eStay,
		eExit
	};

	eCollisionState myCollisionState = eCollisionState::eNone;

	Vector3 myLastPos;

	Vector3 myOffset;
	Vector3 mySize;

	float myMaxStepHeight = 0.2f;
	bool myIsStatic;
	bool myIsTrigger;
	GameObject* myParent;
	Tga2D::Transform myTransform;
	SetOfNormals myNormals;
	SetOfCorners myCorners;
};