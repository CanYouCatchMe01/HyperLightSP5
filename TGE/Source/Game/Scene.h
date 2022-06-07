#pragma once
#include <vector>
#include "RenderObjectManager.h"
#include "CollisionManager.h"
#include <tga2d/graphics/Camera.h>
#include "SpawnPointManager.h"

#include <tga2d/graphics/AmbientLight.h>
#include <tga2d/graphics/DirectionalLight.h>
#include <json.hpp>

class GameObject;

class Scene
{
public:
	friend class UnityLoader;
	friend class RenderManager;
public:
	std::string name;
	Scene(class PollingStation* aPollingStation);
	~Scene();
	void Update(float aTimeDelta);
	void OnStart();
	RenderObjectManager& GetRenderObjectManager();
	CollisionManager& GetCollisionManager();
	SpawnPointManager& GetSpawnPointManager();
	Tga2D::Camera& GetCamera();

	void SetDirectionalLight(const Tga2D::DirectionalLight& aDirectionalLight);
	void SetAmbientLight(const Tga2D::AmbientLight& anAmbientLight);

	GameObject* CreateGameObject(nlohmann::json& json);
	GameObject* CreateGameObject();
	void AddGameObject(GameObject* aGameObject);
	void RemoveGameObject(GameObject* aGameObject);

private:
	//void AddImguiNode(class GameObject* aGameObject); // replaced with a prettier version.
	std::vector<GameObject*> myGameObjects;
	std::vector<GameObject*> myTempAddObjects;
	std::vector<GameObject*> myTempRemoveObjects;

	Tga2D::Camera myCamera;
	Tga2D::DirectionalLight myDirectionalLight;
	Tga2D::AmbientLight myAmbientLight;
	class PollingStation* myPollingStation;
	RenderObjectManager myRenderObjectManager;
	CollisionManager myCollisionManager;
	SpawnPointManager mySpawnPointManager;
};