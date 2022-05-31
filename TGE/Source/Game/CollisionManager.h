#pragma once
#include "OBB3D.h"
#include <set>

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

private:
	std::set<size_t> myEmptyColliders;
	std::vector<OBB3D> myOBBColliders;
};