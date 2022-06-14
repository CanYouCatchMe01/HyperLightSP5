#include "stdafx.h"
#include "TeleportActivator.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameDataManager.h"
#include "PollingStation.h"
#include "EmitterComponent.h"

TeleportActivator::TeleportActivator() 
{}

TeleportActivator::~TeleportActivator()
{}

void TeleportActivator::Activate()
{
	if (myIsActive) return;

	if(myGameObject->GetComponent<EmitterComponent>()!=nullptr)
		myGameObject->GetComponent<EmitterComponent>()->SetEmitterStatus(true);

	GameData& currentData = myPollingStation->myGameDataManager.get()->GetGameData();
	currentData.myTeleporterStatus;

	std::string currentScene = myPollingStation->mySceneManager.get()->GetActiveScene()->name;

	if (currentScene == "Badlands 2" || currentScene == "EmeliesTestScene")
		currentData.myTeleporterStatus[0] = true;
	else if (currentScene == "Badlands 3")
		currentData.myTeleporterStatus[1] = true;
	else if (currentScene == "Jungle 2")
		currentData.myTeleporterStatus[2] = true;
	else if (currentScene == "Jungle 3")
		currentData.myTeleporterStatus[3] = true;
	else if (currentScene == "Hub")
		currentData.myTeleporterStatus[4] = true;

	std::cout << "Current scene: " << currentScene << std::endl;

	myIsActive = true;
}

void TeleportActivator::OnAwake()
{
}

void TeleportActivator::OnStart()
{
	if (myGameObject->GetComponent<EmitterComponent>() != nullptr && !myIsActive)
		myGameObject->GetComponent<EmitterComponent>()->SetEmitterStatus(false);
}

void TeleportActivator::OnUpdate(const float /*aDeltaTime*/)
{
}
