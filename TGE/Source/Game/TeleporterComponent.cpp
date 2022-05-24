#include "stdafx.h"
#include "TeleporterComponent.h"
#include "SceneManager.h"
#include "Scene.h"

TeleporterComponent::TeleporterComponent(std::string aScene, std::string aCheckpointName)
{
	mySceneToLoad = aScene;
	myCheckPointToLoad = aCheckpointName;
}

void TeleporterComponent::Load()
{
	if (myScene->name == mySceneToLoad)
	{
		myPollingStation->myPlayer->GetTransform().SetPosition(myScene->GetSpawnPointManager().GetSpawnPosition(myCheckPointToLoad));
		return;
	}
	myPollingStation->mySceneManager->LoadScene(mySceneToLoad, myCheckPointToLoad);
}

void TeleporterComponent::OnAwake()
{}

void TeleporterComponent::OnStart()
{}

void TeleporterComponent::OnUpdate(const float /*aDeltaTime*/)
{}
