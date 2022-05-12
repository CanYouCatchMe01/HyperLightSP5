#include "stdafx.h"
#include "CollisionManager.h"

CollisionManager::CollisionManager()
{
}

size_t CollisionManager::RegisterBoxCollider(Tga2D::Vector3f aSize, Tga2D::Vector3f aCenter, bool aIsStatic, bool aIsTrigger, GameObject* aParent)
{
	myOBBColliders.push_back(OBB3D(aSize, aCenter, aIsStatic, aIsTrigger, aParent));
	return myOBBColliders.size()-1;
}

OBB3D* CollisionManager::GetOBB3D(size_t aHandle)
{
	return &myOBBColliders[aHandle];
}

void CollisionManager::Update()
{
	/*for (auto& outer : myOBBColliders)
	{
		for (auto& inner : myOBBColliders)
		{
			if (&outer == &inner) { continue; }
			outer.Collides(inner);
		}
	}*/

	for (size_t i = 0; i < myOBBColliders.size(); i++)
	{
		for (size_t j = i+1; j < myOBBColliders.size(); j++)
		{
			myOBBColliders[i].Collides(myOBBColliders[j]);
		}
	}
}

void CollisionManager::Render()
{
	for (auto& c : myOBBColliders)
	{
		c.Draw();
	}
}

