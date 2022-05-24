#pragma once

class GameObject;

class SpawnPointManager
{
public:
	SpawnPointManager();
	void Register(std::string aName, GameObject* aGameObject);
	void Unregister(std::string aName);
	Tga2D::Vector3f GetSpawnPosition(std::string aName);
private:
	std::unordered_map<std::string, GameObject*> mySpawnPoints;
};