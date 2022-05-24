#include "stdafx.h"
#include "SpawnPointComponent.h"
#include "SpawnPointManager.h"
#include "Scene.h"

SpawnPointComponent::SpawnPointComponent(std::string aName) : myName(std::move(aName))
{}

SpawnPointComponent::~SpawnPointComponent()
{
	myScene->GetSpawnPointManager().Unregister(myName);
}

void SpawnPointComponent::OnAwake()
{
	myScene->GetSpawnPointManager().Register(myName, myGameObject);
}

void SpawnPointComponent::OnStart()
{}

void SpawnPointComponent::OnUpdate(const float /*aDeltaTime*/)
{}
