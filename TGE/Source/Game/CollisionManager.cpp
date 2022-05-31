#include "stdafx.h"
#include "CollisionManager.h"

CollisionManager::CollisionManager()
{
}

size_t CollisionManager::RegisterBoxCollider(Tga2D::Vector3f aSize, Tga2D::Vector3f aCenter, bool aIsStatic, bool aIsTrigger, GameObject* aParent)
{
	if (myEmptyColliders.size())
	{
		size_t handle = *std::prev(myEmptyColliders.end());
		myEmptyColliders.erase(std::prev(myEmptyColliders.end()));
		myOBBColliders[handle] = OBB3D(aSize, aCenter, aIsStatic, aIsTrigger, aParent);
		return handle;
	}
	myOBBColliders.push_back(OBB3D(aSize, aCenter, aIsStatic, aIsTrigger, aParent));
	return myOBBColliders.size()-1;
}

void CollisionManager::UnregisterBoxCollider(size_t aHandle)
{
	myEmptyColliders.insert(aHandle);
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
			if (myEmptyColliders.count(i) || myEmptyColliders.count(j))
				continue;

			myOBBColliders[i].Collides(myOBBColliders[j]);
		}
	}
}

void CollisionManager::Render()
{
#ifdef _DEBUG

	for (auto& c : myOBBColliders)
	{
		c.Draw();
	}
#endif // DEBUG
}

