#include "stdafx.h"
#include "CassetTapeComponent.h"
#include "GameDataManager.h"
#include "Scene.h"

CassetTapeComponent::CassetTapeComponent(const int& aNumber) : myNumber(aNumber)
{
}

void CassetTapeComponent::OnAwake()
{
}

void CassetTapeComponent::OnStart()
{
	GameData& gameData = myPollingStation->myGameDataManager.get()->GetGameData();
	
	//If this key is allReady collected then remove it
	if (gameData.myKeys[myNumber])
	{
		myScene->RemoveGameObject(myGameObject);
	}
}

void CassetTapeComponent::OnUpdate(float /*aDeltaTime*/)
{
}

const int& CassetTapeComponent::GetNumber()
{
	return myNumber;
}
