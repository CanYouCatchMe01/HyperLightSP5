#include "stdafx.h"
#include "SceneLoaderComponent.h"
#include "SceneManager.h"

SceneLoaderComponent::SceneLoaderComponent(std::string aScene, std::string aCheckpointName)
{
	mySceneToLoad = aScene;
	myCheckPointToLoad = aCheckpointName;
}

void SceneLoaderComponent::Load()
{
	myPollingStation->mySceneManager->LoadScene(mySceneToLoad);
}

void SceneLoaderComponent::OnAwake()
{}

void SceneLoaderComponent::OnStart()
{}

void SceneLoaderComponent::OnUpdate(const float /*aDeltaTime*/)
{}
