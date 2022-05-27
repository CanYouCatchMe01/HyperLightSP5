#include "stdafx.h"
#include "TeleporterComponent.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameDataManager.h"

TeleporterComponent::TeleporterComponent(std::string aScene)
{
	mySceneName = aScene;
}

void TeleporterComponent::Load()
{
	//if (myScene->name == mySceneToLoad)
	//{
	//	myPollingStation->myPlayer->GetTransform().SetPosition(myScene->GetSpawnPointManager().GetSpawnPosition(myCheckPointToLoad));
	//	return;
	//}
	//myPollingStation->mySceneManager->LoadScene(mySceneToLoad, myCheckPointToLoad);
}

void TeleporterComponent::Activate()
{
	if (myIsActive) return;

	std::string currentScene = myScene->name;
	GameData currentData = myPollingStation->myGameDataManager.get()->GetGameData();
	currentData.TeleporterStatus;

	if (mySceneName == "Badlands 2")
		currentData.TeleporterStatus[0] = true;
	else if (mySceneName == "Badlands 3")
		currentData.TeleporterStatus[1] = true;
	else if (mySceneName == "Jungle 2")
		currentData.TeleporterStatus[2] = true;
	else if (mySceneName == "Jungle 3")
		currentData.TeleporterStatus[3] = true;
	else if (mySceneName == "Hub")
		currentData.TeleporterStatus[4] = true;

	myPollingStation->myGameDataManager.get()->UpdateGameData(currentData);

	myIsActive = true;
}

void TeleporterComponent::OnAwake()
{
}

void TeleporterComponent::OnStart()
{
}

void TeleporterComponent::OnUpdate(const float /*aDeltaTime*/)
{
}
