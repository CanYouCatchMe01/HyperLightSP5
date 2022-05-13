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

			if (ImGui::BeginTabItem("Transform"))
			{
				//Position
				float position[3] = { myTransform.GetPosition().x,myTransform.GetPosition().y,myTransform.GetPosition().z };
				ImGui::DragFloat3("Position", position);
				myTransform.SetPosition({ position[0], position[1], position[2] });

				//Rotation
				float rotation[3] = { myTransform.GetRotation().x,myTransform.GetRotation().y,myTransform.GetRotation().z };
				ImGui::DragFloat3("Rotation", rotation);
				myTransform.SetRotation({ rotation[0], rotation[1], rotation[2] });

				//Scale
				float scale[3] = { myTransform.GetScale().x,myTransform.GetScale().y,myTransform.GetScale().z };
				ImGui::DragFloat3("Scale", scale);
				myTransform.SetScale({ scale[0], scale[1], scale[2] });
				
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
		//Tga2D::Transform old = myChildren[i]->GetTransform();
		//Tga2D::Matrix4x4f global = myChildren[i]->GetTransform().GetMatrix() * myTransform.GetMatrix();
		//myChildren[i]->GetTransform() = Tga2D::Transform(global);

		myChildren[i]->Update(aTimeDelta);

		//Tga2D::Matrix4x4f local = myChildren[i]->GetTransform().GetMatrix() * Tga2D::Matrix4x4f::GetFastInverse(myTransform.GetMatrix());
		//myChildren[i]->GetTransform() = old;
	}
}
