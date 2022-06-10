#pragma once
#include "OBB3D.h"
#include <set>
#include "SparseSet.hpp"

struct Ray
{
	Vector3 origin;
	Vector3 dir;
	float distance;
};

class CollisionManager
{
public:
	CollisionManager();
	~CollisionManager() = default;
	
	size_t RegisterBoxCollider(Tga2D::Vector3f aSize, Tga2D::Vector3f aCenter, bool aIsStatic, bool aIsTrigger, GameObject* aParent = nullptr);
	void UnregisterBoxCollider(size_t aHandle);
	OBB3D* GetOBB3D(size_t aHandle);
	void Update();
	void Render();
	bool TestRay(Ray aRay);
	Vector3 ClosestPoint(Ray aRay, Vector3 pos);

private:
	size_t myNextColliderHandle = 0;
	SparseSet<bool> myIsColliderStatic;
	SparseSet<OBB3D> myOBBColliders;
};