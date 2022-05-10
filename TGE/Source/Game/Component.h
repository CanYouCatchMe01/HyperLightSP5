#pragma once
#include "tga2d/math/Transform.h"
#include "PollingStation.h"
#include "ComponentDebugger.h"

class GameObject;

class Component 
#ifdef _DEBUG
	: ComponentDebugger
#endif // _DEBUG

{
public:

	Component() = default;
	virtual ~Component() = default;
	Component(const Component&) = default;
	Component(Component&&) = default;
	Component& operator=(const Component&) = default;
	Component& operator=(Component&&) = default;

	virtual void OnAwake() = 0;
	virtual void OnStart() = 0;
	virtual void OnUpdate(const float aDeltaTime) = 0;

	virtual void OnCollisionEnter(GameObject* aOther);
	virtual void OnCollisionStay(GameObject* aOther);
	virtual void OnCollisionExit(GameObject* aOther);

	friend GameObject;

protected:

	class Scene* myScene;
	PollingStation* myPollingStation;
	GameObject* myGameObject;
	Tga2D::Transform* myTransform;
};
