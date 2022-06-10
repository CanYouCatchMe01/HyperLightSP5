#include "stdafx.h"
#include "Scene.h"
#include "GameObject.h"
#include <tga2d/graphics/DirectionalLight.h>
#include <tga2d/light/LightManager.h>
#include <json.hpp>
#include "PollingStation.h"
#include "SceneManager.h"

Scene::Scene(PollingStation* aPollingStation) : myPollingStation(aPollingStation), myAmbientLight(Tga2D::Color{ 1, 1, 1 }, 3000.0)
, myDirectionalLight({}, Tga2D::Color{ 1, 1, 1 }, 1.0)
{
	myDirectionalLight = Tga2D::DirectionalLight(
		Tga2D::Transform{
			Tga2D::Vector3f(0, 0, 0),
			Tga2D::Rotator(0, 0, 0)
		},
		Tga2D::Color{ 1, 1, 1 },
		3000
	);
	
	std::wstring cubeMap = L"Sprites/cube_1024_preblurred_angle3_Skansen3.dds";
	myAmbientLight = Tga2D::AmbientLight(
		cubeMap,
		Tga2D::Color{ 1.0f, 1.0f, 1.0f },
		1.0f
	);
}

Scene::~Scene()
{
	for (size_t i = 0; i < myGameObjects.size(); i++)
		delete myGameObjects[i];
}

void Scene::Update(float aTimeDelta)
{
	//Update
	for (size_t i = 0; i < myGameObjects.size(); ++i)
		myGameObjects[i]->Update(aTimeDelta);

	myCollisionManager.Update();

	//Remove objects
	for (auto& gameObject : myTempRemoveObjects)
	{
		auto it = myGameObjects.begin();
		while (it != myGameObjects.end())
		{
			if (*it == gameObject)
			{
				delete *it;
				it = myGameObjects.erase(it);
				break;
			}
			else
			{
				++it;
			}
		}
	}
	myTempRemoveObjects.clear();

	//Add objects
	for (auto& gameObject : myTempAddObjects)
	{
		myGameObjects.push_back(gameObject);
	}

	myTempAddObjects.clear();
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

SpawnPointManager& Scene::GetSpawnPointManager()
{
	return mySpawnPointManager;
}

Tga2D::Camera& Scene::GetCamera()
{
	return myCamera.myCamera;
}

void Scene::SetDirectionalLight(const Tga2D::DirectionalLight& aDirectionalLight)
{
	myDirectionalLight = aDirectionalLight;
	//Tga2D::Engine::GetInstance()->GetLightManager().SetDirectionalLight(myDirectionalLight);
}

void Scene::SetAmbientLight(const Tga2D::AmbientLight& anAmbientLight)
{
	myAmbientLight = anAmbientLight;
	//Tga2D::Engine::GetInstance()->GetLightManager().SetAmbientLight(myAmbientLight);
}

GameObject* Scene::CreateGameObject(nlohmann::json& json)
{
	GameObject* go = myPollingStation->mySceneManager->myUnityLoader.CreateGameObject(json, this);
	myGameObjects.push_back(go);
	return go;
}

GameObject* Scene::CreateGameObject()
{
	myGameObjects.push_back(new GameObject(this));
	return myGameObjects.back();
}

void Scene::AddGameObject(GameObject* aGameObject)
{
	myTempAddObjects.push_back(aGameObject);
}

void Scene::RemoveGameObject(GameObject* aGameObject)
{
	myTempRemoveObjects.push_back(aGameObject);
}
