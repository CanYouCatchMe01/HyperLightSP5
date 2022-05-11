#include "stdafx.h"
#include "Scene.h"
#include "GameObject.h"
#include <tga2d/graphics/DirectionalLight.h>
#include <tga2d/light/LightManager.h>
#include <json.hpp>
#include "PollingStation.h"

Scene::Scene(PollingStation* aPollingStation) : myPollingStation(aPollingStation)
{

	myDirectionalLight = new Tga2D::DirectionalLight(
		Tga2D::Transform{
			Tga2D::Vector3f(0, 0, 0),
			Tga2D::Rotator(0, 0, 0)
		},
		Tga2D::Color{ 1, 1, 1 },
		3000
	);
	


	std::wstring cubeMap = L"Sprites/cube_1024_preblurred_angle3_Skansen3.dds";
	myAmbientLight = new Tga2D::AmbientLight(
		cubeMap,
		Tga2D::Color{ 1.0f, 1.0f, 1.0f },
		1.0f
	);

	Tga2D::Engine::GetInstance()->GetLightManager().SetDirectionalLight(*myDirectionalLight);
	Tga2D::Engine::GetInstance()->GetLightManager().SetAmbientLight(*myAmbientLight);

}

Scene::~Scene()
{
	delete myDirectionalLight;
}

void Scene::Update(float aTimeDelta)
{
	

	for (size_t i = 0; i < myGameObjects.size(); ++i)
		myGameObjects[i]->Update(aTimeDelta);

	myCollisionManager.Update();

	//if (ImGui::Begin("Object Hierarchy")) // Removed for a prettier debugger.
	//{
	//	for (auto& go : myGameObjects)
	//	{
	//		AddImguiNode(go);
	//	}

	//}
	//ImGui::End();
}

void Scene::AddImguiNode(GameObject* aGameObject)
{
	if (ImGui::TreeNode(aGameObject->name.c_str()))
	{
		Tga2D::Transform& transform = aGameObject->GetTransform();
		ImGui::Text("Tranform");
		ImGui::DragFloat3("position", (float*)&transform, 0.1f);
		ImGui::DragFloat3("rotation", (float*)(&transform) + 3, 0.1f);
		ImGui::DragFloat3("scale", (float*)(&transform) + 6, 0.1f);

		ImGui::Separator();
		ImGui::Text("Children");
		for (auto& child : aGameObject->myChildren)
		{
			AddImguiNode(child);
		}

		ImGui::TreePop();
	}
}

void Scene::OnStart()
{
	for (size_t i = 0; i < myGameObjects.size(); ++i)
		myGameObjects[i]->OnStart();
}

RenderObjectManager& Scene::GetRenderObjectManager()
{
	return myRenderObjectManager;
}

CollisionManager& Scene::GetCollisionManager()
{
	return myCollisionManager;
}

Tga2D::Camera& Scene::GetCamera()
{
	return myCamera;
}