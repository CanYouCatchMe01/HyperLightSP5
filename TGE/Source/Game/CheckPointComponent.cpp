#include "stdafx.h"
#include "PlayerComponent.h"
#include "CheckPointComponent.h"
#include "GameObject.h"
#include "Scene.h"

CheckPointComponent::CheckPointComponent(std::string aSceneName, std::string aCheckPointName) :
	mySceneName(std::move(aSceneName)), myCheckPointName(std::move(aCheckPointName))
{}

CheckPointComponent::~CheckPointComponent()
{
}

void CheckPointComponent::OnAwake()
{
}

void CheckPointComponent::OnStart()
{
	myPlayerData.mySavePosition = myGameObject->GetTransform().GetPosition();
}

void CheckPointComponent::OnUpdate(const float /*aDeltaTime*/)
{
}

void CheckPointComponent::Save()
{

	// TODO: Save game state
	std::cout << "Saving player\n";
	GameData& gM = myPollingStation->myGameDataManager.get()->GetGameData();
	gM.myCheckpoint = this;
	PlayerComponent* player = myPollingStation->myPlayer->GetComponent<PlayerComponent>();
	myPlayerData.myCurrentHP = player->myPlayerData.myCurrentHP;
	myPlayerData.myMaxHP = player->myPlayerData.myMaxHP;

	/*player->myPlayerData.mySavePosition*/
	/*player->SetPosition(player->myPlayerData.myCheckpoint->myTransform->GetPosition());*/
	//PlayerData playerData(player);
	//myPlayerData = playerData;
}

void CheckPointComponent::Load()
{
	auto player = myPollingStation->myPlayer->GetComponent<PlayerComponent>();
	player->myPlayerData = myPlayerData;
	myPollingStation->myPlayer->GetTransform().SetPosition(myScene->GetSpawnPointManager().GetSpawnPosition(myCheckPointName));

}
