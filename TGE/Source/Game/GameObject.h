#pragma once
#include <vector>
#include <tga2d/math/Transform.h>
#include "Component.h"
#include "BoxColliderComponent.h"
#ifdef _DEBUG
#include "DebugObserver.h"
#endif // _DEBUG

namespace CapNCrunch
{
	class UnityLoader;
}

class Emitter;

class GameObject
#ifdef _DEBUG
	: DebugObserver
#endif // _DEBUG
{
public:
	friend class Scene;
	friend class UnityLoader;

	inline GameObject(class Scene* aScene):
		myScene(aScene) {}

	GameObject(const GameObject& aRhs);
	GameObject(GameObject&& aRhs) noexcept;
	GameObject& operator=(const GameObject& aRhs);
	GameObject& operator=(GameObject&& aRhs) noexcept;

	void OnStart();

	template <typename ComponentType, typename... Args>
	ComponentType* AddComponent(Args&&... args)
	{
		ComponentType* c = new ComponentType(args...);
		c->myTransform = &myTransform;
		c->myGameObject = this;
		c->myPollingStation = myPollingStation;
		c->myScene = myScene;
		myComponents.push_back(c);
		c->OnAwake();

		return c;
	}

	template <typename ComponentType>
	ComponentType* GetComponent()
	{
		for (auto component : myComponents)
		{
			ComponentType* c = dynamic_cast<ComponentType*>(component);
			if (c)
			{
				return c;
			}
		}

		return nullptr;
	}

	template <typename ComponentType>
	void RemoveComponent()
	{
		for (size_t i = 0; i < myComponents.size(); ++i)
		{
			ComponentType* c = dynamic_cast<ComponentType*>(myComponents[i]);
			if (c)
			{
				std::swap(myComponents[i], myComponents.back());
				delete myComponents.back();
				myComponents.pop_back();
				return;
			}
		}
	}

	void OnCollisionEnter(GameObject* aOther);
	void OnCollisionStay(GameObject* aOther);
	void OnCollisionExit(GameObject* aOther);

	inline Tga2D::Transform& GetTransform() { return myTransform; }
	inline Scene* GetParentScene() { return myScene; }
	PollingStation* GetPollingStation() { return myPollingStation; }
#ifdef _DEBUG
	void DebugUpdate() override;
#endif // _DEBUG
protected:
	virtual ~GameObject(); //Protected för att bara en scen för förstöra gameobjects, mvh elias
private:
	void Update(float aTimeDelta);
	std::vector<GameObject*> myChildren;
	PollingStation* myPollingStation;
	Scene* myScene;
	Tga2D::Transform myTransform;
	std::vector<class Component*> myComponents;
#ifdef _DEBUG
	bool myPoppedOut = false;
#endif //_DEBUG
public:
	std::string name;
	std::string tag;
};