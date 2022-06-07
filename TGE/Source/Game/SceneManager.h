#pragma once
#include <vector>
#include <memory>
#include "UnityLoader.h"
#include "RenderManager.h"

class PollingStation;

class SceneManager
{
public:
	friend class Scene;
	SceneManager(PollingStation* aPollingStation);
	~SceneManager();

	void Update(float aTimeDelta);
	void Render();
	void LoadScene(std::string aScenePath);
	void LoadScene(std::string aScenePath, std::string aCheckPoint);
	void UnloadAllScenes();

	inline const class Scene* GetActiveScene() const { return myScenes[myActiveScene]; };
	std::vector<class Scene*> myScenes;
	std::vector<std::string> myScenePaths;
	
private:
	UnityLoader myUnityLoader;
	int32_t mySceneToDelete = -1; // Used to delete scene after the update
	int32_t myActiveScene = -1;
	RenderManager myRenderManager;
	PollingStation* myPollingStation;
};