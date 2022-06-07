#include "stdafx.h"
#include "SceneManager.h"
#include "Scene.h"
#include "PollingStation.h"

SceneManager::SceneManager(PollingStation* aPollingStation) : myUnityLoader(aPollingStation), myPollingStation(aPollingStation)
{
	for (const auto& entry : std::filesystem::directory_iterator("Assets/Scenes"))
	{
		std::string entryString = entry.path().string();
		std::string baseFilename = entryString.substr(entryString.find_last_of("\\") + 1);
		size_t jsonStart = baseFilename.find(".json");
		baseFilename = baseFilename.substr(0, jsonStart);
		myScenePaths.push_back(baseFilename);
		// martin was here
	}
}

SceneManager::~SceneManager()
{
	for (auto& scene : myScenes)
		delete scene;
}

void SceneManager::Update(float aTimeDelta)
{
	if (myScenes.size() == 0)
		return;

	myScenes[myActiveScene]->Update(aTimeDelta);

	if (mySceneToDelete >= 0)
	{
		delete myScenes[mySceneToDelete];
		myScenes.erase(myScenes.begin() + mySceneToDelete);
		myActiveScene = 0; //quick implement, maybe dont just set to 0
		mySceneToDelete = -1;
	}
}

void SceneManager::Render()
{
	if (myScenes.size() == 0)
		return;

	myRenderManager.Render(myScenes[myActiveScene]);
}

void SceneManager::LoadScene(std::string aScenePath)
{
	if (myScenes.size())
		mySceneToDelete = myActiveScene;
	
	myScenes.push_back(myUnityLoader.CreateScene(aScenePath));
	myActiveScene = static_cast<int32_t>(myScenes.size()) - 1;
	myScenes[myActiveScene]->OnStart();
}

void SceneManager::LoadScene(std::string aScenePath, std::string aCheckPoint)
{
	if (myScenes.size())
		mySceneToDelete = myActiveScene;
		

	myScenes.push_back(myUnityLoader.CreateScene(aScenePath));
	myActiveScene = static_cast<int32_t>(myScenes.size()) - 1;
	myScenes[myActiveScene]->OnStart();
	auto pos = myScenes[myActiveScene]->GetSpawnPointManager().GetSpawnPosition(aCheckPoint);
	myPollingStation->myPlayer->GetTransform().SetPosition(pos);
}

void SceneManager::UnloadAllScenes()
{
	for (auto& scene : myScenes)
		delete scene;
	myScenes.clear();
	myActiveScene = -1;
}
