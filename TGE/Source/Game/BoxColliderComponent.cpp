#include "stdafx.h"
#include "BoxColliderComponent.h"
#include "Scene.h"

BoxColliderComponent::BoxColliderComponent(Tga2D::Vector3f aSize, Tga2D::Vector3f aCenter, bool aIsStatic, bool aIsTrigger) : mySize(aSize), myCenter(aCenter), myIsStatic(aIsStatic), myIsTrigger(aIsTrigger)
{}

BoxColliderComponent::~BoxColliderComponent()
{
	myScene->GetCollisionManager().UnregisterBoxCollider(myHandle);
}

void BoxColliderComponent::OnAwake()
{
	myHandle = myScene->GetCollisionManager().RegisterBoxCollider(mySize, myCenter, myIsStatic, myIsTrigger, myGameObject);
}

void BoxColliderComponent::OnStart()
{
}

void BoxColliderComponent::OnUpdate(const float /*adeltatime*/)
{
	myScene->GetCollisionManager().GetOBB3D(myHandle)->SetTransform(*myTransform);
}

void BoxColliderComponent::SetAlwaysSendEvent(bool aState)
{
	myScene->GetCollisionManager().GetOBB3D(myHandle)->myAlwaysSendEvent = aState;
}

#ifdef _DEBUG
void BoxColliderComponent::DebugUpdate()
{
	if (ImGui::Checkbox("Draw collider", &myDrawHitbox))
	{
		myScene->GetCollisionManager().GetOBB3D(myHandle)->myDrawHitbox = myDrawHitbox;
	}
}
#endif