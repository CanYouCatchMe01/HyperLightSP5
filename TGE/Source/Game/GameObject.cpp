#include "stdafx.h"
#include "GameObject.h"
#include <tga2d/engine.h>
#include <tga2d/drawers/DebugDrawer.h>
#ifdef _DEBUG
#include "BaseDebugger.h"
#include "imgui/imgui.h"
#endif //_DEBUG

void GameObject::OnCollisionEnter(GameObject* aOther)
{
	for (size_t i = 0; i < myComponents.size(); ++i)
	{
		myComponents[i]->OnCollisionEnter(aOther);
	}
}

void GameObject::OnCollisionStay(GameObject* aOther)
{
	for (size_t i = 0; i < myComponents.size(); ++i)
	{
		myComponents[i]->OnCollisionStay(aOther);
	}
}

void GameObject::OnCollisionExit(GameObject* aOther)
{
	for (size_t i = 0; i < myComponents.size(); ++i)
	{
		myComponents[i]->OnCollisionExit(aOther);
	}
}

GameObject::~GameObject()
{
#ifdef _DEBUG
	myPollingStation->myDebugger.get()->RemoveObserver(this);
#endif // _DEBUG
	for (auto& component : myComponents)
	{
		delete component;
	}
	myComponents.clear();
}

GameObject::GameObject(const GameObject& aRhs)
{
	myTransform = aRhs.myTransform;
	myComponents = aRhs.myComponents;
}

GameObject::GameObject(GameObject&& aRhs) noexcept
{
	myTransform = aRhs.myTransform;
	myComponents = aRhs.myComponents;
	aRhs.myComponents.clear();
}

GameObject& GameObject::operator=(const GameObject& aRhs)
{
	myTransform = aRhs.myTransform;
	myComponents = aRhs.myComponents;
	return *this;
}

GameObject& GameObject::operator=(GameObject&& aRhs) noexcept
{
	myComponents = aRhs.myComponents;
	aRhs.myComponents.clear();
	return *this;
}

void GameObject::OnStart()
{
	for (size_t i = 0; i < myComponents.size(); ++i)
	{
		myComponents[i]->OnStart();
	}

	for (size_t i = 0; i < myChildren.size(); ++i)
	{
		myChildren[i]->OnStart();
	}
}

#ifdef _DEBUG
void GameObject::DebugUpdate()
{
	ImGui::Checkbox(name.c_str(), &myPoppedOut);
	if (myPoppedOut)
	{

		if (!ImGui::Begin(name.c_str()))
		{
			ImGui::End();
			return;
		}
		if (ImGui::BeginTabBar("Boop"))
		{
			if (ImGui::BeginTabItem("Components"))
			{
				for (Component* i : myComponents)
				{
					i->DebugUpdate();
				}
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Children"))
			{
				for (GameObject* i : myChildren)
				{
					i->DebugUpdate();
				}
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
		ImGui::End();
	}
}
#endif // _DEBUG



void GameObject::Update(float aTimeDelta)
{
	for (size_t i = 0; i < myComponents.size(); ++i)
	{
		myComponents[i]->OnUpdate(aTimeDelta);
	}

	for (size_t i = 0; i < myChildren.size(); ++i)
	{
		myChildren[i]->Update(aTimeDelta);
	}
}
