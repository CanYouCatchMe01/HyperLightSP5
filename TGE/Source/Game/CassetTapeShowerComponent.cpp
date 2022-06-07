#include "stdafx.h"
#include "CassetTapeShowerComponent.h"
#include "GameDataManager.h"
#include "Scene.h"
#include "SceneManager.h"

CassetTapeShowerComponent::CassetTapeShowerComponent(const float aWaitSeconds, const nlohmann::json& aSpawnObject) : 
	myWaitSeconds(aWaitSeconds),
	mySpawnObject(aSpawnObject)
{

}

void CassetTapeShowerComponent::OnAwake()
{
}

void CassetTapeShowerComponent::OnStart()
{
	auto& gameData = myPollingStation->myGameDataManager.get()->GetGameData();
	if (gameData.myKeys.all()) //All keys collected
	{
		myScene->CreateGameObject(mySpawnObject);

		//sets the load scene timer
		myTimer.SetDuration(5.0f);
		myTimer.SetCallback([this]()
			{
				//Load Win UI
				myPollingStation->mySceneManager.get()->LoadScene("MarcusTest2");
			});
		myTimer.Start();
	}
}

void CassetTapeShowerComponent::OnUpdate(float aDeltaTime)
{
	myTimer.Update(aDeltaTime);
}
