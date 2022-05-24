#include "stdafx.h"
#include "SpawnPointManager.h"
#include "GameObject.h"

SpawnPointManager::SpawnPointManager()
{

}

void SpawnPointManager::Register(std::string aName, GameObject* aGameObject)
{
	mySpawnPoints.insert({ aName, aGameObject });
}

void SpawnPointManager::Unregister(std::string aName)
{
	mySpawnPoints.erase(aName);
}

Tga2D::Vector3f SpawnPointManager::GetSpawnPosition(std::string aName)
{
	return mySpawnPoints[aName]->GetTransform().GetPosition();
}
