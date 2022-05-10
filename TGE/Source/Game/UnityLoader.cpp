#include "stdafx.h"
#include "UnityLoader.h"
#include "Scene.h"
#include <fstream>
#include <json.hpp>
#include "GameObject.h"
#include "PollingStation.h"
#include <string>
#include "MeshComponent.h"
#include "CameraComponent.h"
#include "Converter.h"
#include "PlayerComponent.h"
#include "EnemyComponent.h"
#include "BoxColliderComponent.h"
#include "BaseDebugger.h"

UnityLoader::UnityLoader(PollingStation* aPollingStation)
{
	myPollingStation = aPollingStation;
}

UnityLoader::~UnityLoader()
{
}

class Scene* UnityLoader::CreateScene(std::string aPathToJson)
{
	Scene* scene = new Scene(myPollingStation);
	nlohmann::json j = nlohmann::json::parse(std::ifstream("Assets/Scenes/" + aPathToJson + ".json"));

	for (auto& gameObject : j["gameObjects"])
	{
		scene->myGameObjects.push_back(CreateGameObject(gameObject, scene));
		myPollingStation->myDebugger.get()->AddObserver(scene->myGameObjects.back());
	}

	return scene;
}

GameObject* UnityLoader::CreateGameObject(nlohmann::json& aGameObject, class Scene* aScene)
{
	GameObject* gameObject = new GameObject(aScene);

	gameObject->name = aGameObject["name"];
	gameObject->tag = aGameObject["tag"];
	gameObject->myPollingStation = aScene->myPollingStation;

	gameObject->GetTransform() = CreateTransform(aGameObject["transform"]);

	for (auto& child : aGameObject["children"])
	{
		gameObject->myChildren.push_back(CreateGameObject(child, aScene));
	}

	for (auto& comp : aGameObject["components"])
	{
		std::string type = comp["type"];
		auto data = comp["data"];
		
		if (type == "mesh")
		{
			gameObject->AddComponent<MeshComponent>(data["model"], data["albedo"], data["normal"], data["reflective"], aScene);
		}
		else if (type == "camera")
		{
			gameObject->AddComponent<CameraComponent>(&aScene->myCamera, data["fov"]);
		}
		else if (type == "player")
		{
			gameObject->AddComponent<PlayerComponent>(data["max_hp"], data["max_healing"], data["max_attacks"], data["dash_time"], data["healing_time"], data["attack_time"], data["speed"], data["dash_speed"]);
		}
		else if (type == "box_collider")
		{
			gameObject->AddComponent<BoxColliderComponent>(data["size"], data["center"], aGameObject["is_static"], data["is_trigger"]);
			// TODO: Add box collider
		}
		else if (type == "enemy")
		{
			gameObject->AddComponent<EnemyComponent>(data["type"], data["max_hp"], data["speed"], data["attack_speed"], data["detection_radius"]);
		}
	}

	return gameObject;
}

Tga2D::Transform UnityLoader::CreateTransform(nlohmann::json& aTransform)
{
	Tga2D::Vector3f pos = { aTransform["position"]["x"], aTransform["position"]["y"], aTransform["position"]["z"] };
	Tga2D::Quaternionf rot = { aTransform["rotation"]["w"], aTransform["rotation"]["x"], aTransform["rotation"]["y"], aTransform["rotation"]["z"]};
	Tga2D::Vector3f	scale = { aTransform["scale"]["x"], aTransform["scale"]["y"], aTransform["scale"]["z"] };

	Tga2D::Transform transform(pos, rot, scale);

	return transform;
}
