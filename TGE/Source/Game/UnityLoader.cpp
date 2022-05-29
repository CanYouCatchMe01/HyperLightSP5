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
#include "PopcornEnemy.h"
#include "ChargeEnemy.h"
#include "FluteEnemy.h"
#include "BoxColliderComponent.h"
#include "AnimatedMeshComponent.h"
#include "MeleeComponent.h"
#include "TeleporterComponent.h"
#include "SpawnPointComponent.h"
#include "CheckPointComponent.h"
#include "MusicChangeComponent.h"
#include "PointLightComponent.h"
#include "tga2d/graphics/DirectionalLight.h"
#include "tga2d/graphics/AmbientLight.h"

#ifdef _DEBUG
#include "BaseDebugger.h"
#endif // _DEBUG



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

	try // Temporary
	{
		scene->name = j["name"];
	}
	catch (const std::exception&)
	{
		ERROR_PRINT("Scene outdated, please export");
	}
	

	for (auto& gameObject : j["gameObjects"])
	{
		scene->myGameObjects.push_back(CreateGameObject(gameObject, scene));
#ifdef _DEBUG
		myPollingStation->myDebugger.get()->AddObserver(scene->myGameObjects.back());
#endif // _DEBUG
	}

	return scene;
}

GameObject* UnityLoader::CreateGameObject(nlohmann::json& aGameObject, class Scene* aScene)
{
	GameObject* gameObject = new GameObject(aScene);

	gameObject->name = aGameObject["name"];

	//Need try and catch for old scenes, that has a uses a "string", instead of an "int"
	try
	{
		gameObject->tag = aGameObject["tag"].get<eTag>();
		gameObject->layer = aGameObject["layer"].get<eLayer>();
	}
	catch (const std::exception&)
	{
		ERROR_PRINT("%s", "You need to re-export your scene! No tag or layer found in json");
	}
	
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
		
		try //Trying if a component is crashing
		{
			if (type == "mesh")
			{
				gameObject->AddComponent<MeshComponent>(data["model"], data["albedo"], data["normal"], data["reflective"], aScene);
			}
			else if (type == "animated_mesh")
			{
				gameObject->AddComponent<AnimatedMeshComponent>(data["model"], data["albedo"], data["normal"], data["reflective"], data["animations"], aScene);
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
			else if (type == "popcorn_enemy")
			{
				gameObject->AddComponent<PopcornEnemy>(data["max_hp"], data["speed"], data["attack_speed"], data["detection_radius"], data["idle_speed"], data["attack_damage"]);
			}
			else if (type == "charge_enemy")
			{
				gameObject->AddComponent<ChargeEnemy>(data["max_hp"], data["speed"], data["attack_speed"], data["detection_radius"], data["charge_radius"], data["charge_time"], data["idle_speed"], data["attack_damage"], data["dash_speed"]);
			}
			else if (type == "flute_enemy")
			{
				gameObject->AddComponent<FluteEnemy>(data["max_hp"], data["speed"], data["attack_speed"], data["detection_radius"], data["idle_speed"], data["attack_damage"]);
			}
			else if (type == "weapon")
			{
				gameObject->AddComponent<MeleeComponent>(data["weapon_upgrade"], data["weapon_damage"]);
			}
			else if (type == "teleporter")
			{
				//	gameObject->AddComponent<TeleporterComponent>(data["scene"], data["spawnpoint"]);
			}
			else if (type == "teleport_1")
			{
				gameObject->AddComponent<TeleporterComponent>(data["scene"]);
			}
			else if (type == "spawn_point")
			{
				gameObject->AddComponent<SpawnPointComponent>(data["name"]);
			}
			else if (type == "check_point")
			{
				gameObject->AddComponent<CheckPointComponent>(data["scene"], data["name"]);
			}
			else if (type == "music_changer")
			{
				gameObject->AddComponent<MusicChangeComponent>(data["music_to_play"]);
			}
			else if (type == "light")
			{
				switch (data["type"].get<eLight>())
				{
				case eLight::Directional:
					aScene->SetDirectionalLight(Tga2D::DirectionalLight(gameObject->GetTransform(), data["color"], data["intensity"]));
					break;
				case eLight::Point:
					gameObject->AddComponent<PointLightComponent>(data["color"], data["intensity"], data["range"]);
					break;
				default:
					break;
				}
			}
			else if (type == "ambient_light")
			{
				std::string cubeMap = data["cube_map"]["texture"];
				//string to wstring
				std::wstring wCubeMap(cubeMap.begin(), cubeMap.end());

				std::wstring cubeMapZoo = L"Assets/Art/2D/earth-cubemap.dds";

				aScene->SetAmbientLight(Tga2D::AmbientLight(wCubeMap, data["color"].get<Tga2D::Color>(), data["intensity"].get<float>()));
			}
		}
		catch (const std::exception&)
		{
			ERROR_PRINT("%s %s %s", "Component", type.c_str(), "crashed. The JSON file or the code is not right");
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
