#include "stdafx.h"
#include "CollisionManager.h"

CollisionManager::CollisionManager()
{
}

size_t CollisionManager::RegisterBoxCollider(Tga2D::Vector3f aSize, Tga2D::Vector3f aCenter, bool aIsStatic, bool aIsTrigger, GameObject* aParent)
{
	size_t handle = myOBBColliders.Size();
	myIsColliderStatic.Insert(aIsStatic, handle);
	myOBBColliders.Insert(OBB3D(aSize, aCenter, aIsStatic, aIsTrigger, aParent), handle);
	return handle;
}

void CollisionManager::UnregisterBoxCollider(size_t aHandle)
{
	myIsColliderStatic.Remove(aHandle);
	myOBBColliders.Remove(aHandle);
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

	for (size_t i = 0; i < myOBBColliders.Size(); i++)
	{
		for (size_t j = i+1; j < myOBBColliders.Size(); j++)
		{
			if (myIsColliderStatic[i] && myIsColliderStatic[j])
				continue;

			myOBBColliders[i].Collides(myOBBColliders[j]);
		}
	}

	for (size_t i = 0; i < myOBBColliders.Size(); i++)
	{
		myOBBColliders[i].CallGrounded();
		myOBBColliders[i].myIsGrounded = false;
	}
}

void CollisionManager::Render()
{
#ifdef _DEBUG
	for (size_t i = 0; i < myOBBColliders.Size(); i++)
	{
		myOBBColliders[i].Draw();
	}
#endif // DEBUG
}

bool CollisionManager::TestRay(Ray aRay)
{
	for (size_t i = 0; i < myOBBColliders.Size(); ++i)
	{
		OBB3D& obb = myOBBColliders[i];
		Vector3 x = ClosestPoint(aRay, obb.AveragePos());

		if ((aRay.origin - x).Length() > aRay.distance)
			continue;

		Vector3 p1 = obb.myCorners[6];
		Vector3 p2 = obb.myCorners[4];
		Vector3 p4 = obb.myCorners[7];
		Vector3 p5 = obb.myCorners[2];

		Vector3 u = p1 - p2;
		Vector3 v = p1 - p4;
		Vector3 w = p1 - p5;

		//float udotx = u.Dot(x);
		//float vdotx = v.Dot(x);
		//float wdotx = w.Dot(x);

		/*if (u.Dot(p1) < udotx && udotx < u.Dot(p2))
		{
			if (v.Dot(p1) < vdotx && vdotx < v.Dot(p4))
			{
				if (w.Dot(p1) < wdotx && wdotx < w.Dot(p5))
				{
					return true;
				}
			}
		}*/

		if (p1.Dot(x) > p5.Dot(x))
		{
			if (p1.Dot(x) > p4.Dot(x))
			{
				return true;
			}
		}

	}
	return false;
}

Vector3 CollisionManager::ClosestPoint(Ray aRay, Vector3 pos)
{	
	Vector3 e = pos - aRay.origin;
	const float a = e.Dot(aRay.dir.GetNormalized());
	return aRay.origin + aRay.dir.GetNormalized() * a;
}

