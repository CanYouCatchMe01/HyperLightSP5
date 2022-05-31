#include "stdafx.h"
#include "TeleportActivator.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameDataManager.h"
#include "PollingStation.h"

TeleportActivator::TeleportActivator() 
{}

TeleportActivator::~TeleportActivator()
{}

void TeleportActivator::Activate()
{
	if (myIsActive) return;

	GameData currentData = myPollingStation->myGameDataManager.get()->GetGameData();
	currentData.TeleporterStatus;

	std::string currentScene = myPollingStation->mySceneManager.get()->GetActiveScene()->name;

	if (currentScene == "Badlands 2" || currentScene == "EmeliesTestScene")
		currentData.TeleporterStatus[0] = true;
	else if (currentScene == "Badlands 3")
		currentData.TeleporterStatus[1] = true;
	else if (currentScene == "Jungle 2")
		currentData.TeleporterStatus[2] = true;
	else if (currentScene == "Jungle 3")
		currentData.TeleporterStatus[3] = true;
	else if (currentScene == "Hub")
		currentData.TeleporterStatus[4] = true;

	std::cout << "Current scene: " << currentScene << std::endl;
	currentData;
	myPollingStation->myGameDataManager.get()->UpdateGameData(currentData);

	myIsActive = true;
}

void TeleportActivator::OnAwake()
{
}

void TeleportActivator::OnStart()
{
}

void TeleportActivator::OnUpdate(const float /*aDeltaTime*/)
{
}
