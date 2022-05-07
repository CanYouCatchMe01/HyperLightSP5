#pragma once
#include <vector>
#include "RenderObjectManager.h"
#include "CollisionManager.h"
#include <tga2d/graphics/Camera.h>

namespace Tga2D
{
	class DirectionalLight;
	class AmbientLight;
}

class Scene
{
public:
	friend class UnityLoader;
	friend class RenderManager;
public:
	Scene(class PollingStation* aPollingStation);
	~Scene();
	void Update(float aTimeDelta);
	void OnStart();
	RenderObjectManager& GetRenderObjectManager();
	CollisionManager& GetCollisionManager();
	Tga2D::Camera& GetCamera();
private:
	void AddImguiNode(class GameObject* aGameObject);
	std::vector<class GameObject*> myGameObjects;

	Tga2D::Camera myCamera;
	Tga2D::DirectionalLight* myDirectionalLight;
	Tga2D::AmbientLight* myAmbientLight;
	class PollingStation* myPollingStation;
	RenderObjectManager myRenderObjectManager;
	CollisionManager myCollisionManager;
};