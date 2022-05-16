#include "stdafx.h"
#include "TeleporterComponent.h"
#include "SceneManager.h"

TeleporterComponent::TeleporterComponent(std::string aScene, std::string aCheckpointName)
{
	mySceneToLoad = aScene;
	myCheckPointToLoad = aCheckpointName;
}

void TeleporterComponent::Load()
{
	myPollingStation->mySceneManager->LoadScene(mySceneToLoad);
}

void TeleporterComponent::OnAwake()
{}

void TeleporterComponent::OnStart()
{}

void TeleporterComponent::OnUpdate(const float /*aDeltaTime*/)
{}
