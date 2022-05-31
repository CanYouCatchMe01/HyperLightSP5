#include "stdafx.h"
#include "PlayerComponent.h"
#include "CheckPointComponent.h"
#include "GameObject.h"

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
}

void CheckPointComponent::OnUpdate(const float /*aDeltaTime*/)
{
}

void CheckPointComponent::Save()
{
	// TODO: Save game state
	std::cout << "Saving player\n";
	PlayerComponent* player = myPollingStation->myPlayer->GetComponent<PlayerComponent>(); 
	player->myPlayerData.myCheckpoint = this;
	myPlayerData.CurrentHP = player->myPlayerData.CurrentHP;
	/*player->myPlayerData.mySavePosition*/
	/*player->SetPosition(player->myPlayerData.myCheckpoint->myTransform->GetPosition());*/
	//PlayerData playerData(player);
	//myPlayerData = playerData;
}

void CheckPointComponent::Load()
{
	myPollingStation->myPlayer->GetComponent<PlayerComponent>()->myPlayerData = myPlayerData;
}
