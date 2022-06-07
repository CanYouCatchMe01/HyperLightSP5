#include "stdafx.h"
#include "TeleporterComponent.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameDataManager.h"
#include "PlayerComponent.h"
#include "CheckPointComponent.h"

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
		myPollingStation->myPlayer->GetComponent<PlayerComponent>()->SetFullHP();
		return;
	}
	myPollingStation->mySceneManager->LoadScene(mySceneToLoad, myCheckPointToLoad);
}

void TeleporterComponent::OnAwake()
{
	myGameObject->AddComponent<CheckPointComponent>(mySceneToLoad, myCheckPointToLoad);
}

void TeleporterComponent::OnStart()
{
}

void TeleporterComponent::OnUpdate(const float /*aDeltaTime*/)
{
}