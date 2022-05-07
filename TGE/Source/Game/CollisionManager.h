#pragma once
#include "OBB3D.h"

class CollisionManager
{
public:
	CollisionManager();
	~CollisionManager() = default;
	
	size_t RegisterBoxCollider(Tga2D::Vector3f aSize, Tga2D::Vector3f aCenter, bool aIsStatic, bool aIsTrigger, GameObject* aParent = nullptr);
	OBB3D* GetOBB3D(size_t aHandle);
	void Update();
	void Render();

private:
	std::vector<OBB3D> myOBBColliders;
};