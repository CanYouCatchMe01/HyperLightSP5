#include "stdafx.h"
#include "SceneManager.h"
#include "Scene.h"
#include "PollingStation.h"

SceneManager::SceneManager(PollingStation* aPollingStation) : myUnityLoader(aPollingStation)
{
	LoadScene("ViktorTestScene");

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
	/*static float timer;
	timer += aTimeDelta;
	if (timer > 1.0f)
	{
		std::cout << 1 / aTimeDelta << '\n';
		timer = 0.0f;
	}*/
	
	myScenes[myActiveScene]->Update(aTimeDelta);

//#ifdef _DEBUG // Replaced with prettier version.
//	ImGui::Begin("Scenes");
//	for (auto& scene : myScenePaths)
//	{
//		if (ImGui::Button(scene.c_str()))
//		{
//			if (myScenes.size() > 0)
//			{
//				delete myScenes[0];
//			}
//			myScenes[0] = myUnityLoader.CreateScene(scene);
//			myScenes[0]->OnStart();
//		}
//	}
//	ImGui::End();
//#endif // _DEBUG
}

void SceneManager::Render()
{
	/*if (myScenes.size() == 0)
		return;*/

	myRenderManager.Render(myScenes[myActiveScene]);
}

void SceneManager::LoadScene(std::string aScenePath)
{
	myScenes.push_back(myUnityLoader.CreateScene(aScenePath));
	myActiveScene = myScenes.size() - 1;
	myScenes[myActiveScene]->OnStart();
}
