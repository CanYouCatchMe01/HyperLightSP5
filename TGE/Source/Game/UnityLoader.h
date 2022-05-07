#pragma once
#include <memory>
#include <string>
#include <json.hpp>
#include <tga2d/math/Transform.h>

class UnityLoader
{
public:
	UnityLoader(class PollingStation* aPollingStation);
	~UnityLoader();

	class Scene* CreateScene(std::string aPathToJson);
private:
	class GameObject* CreateGameObject(nlohmann::json& aGameObject, class Scene* aScene);
	Tga2D::Transform CreateTransform(nlohmann::json& aTransform);

private:
	class PollingStation* myPollingStation;
};