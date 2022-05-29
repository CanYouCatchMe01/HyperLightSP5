#pragma once
#include <vector>
#include <memory>
#include "UnityLoader.h"
#include "RenderManager.h"

class PollingStation;

class SceneManager
{
public:
	SceneManager(PollingStation* aPollingStation);
	~SceneManager();

	void Update(float aTimeDelta);
	void Render();
	void LoadScene(std::string aScenePath);
	void LoadScene(std::string aScenePath, std::string aCheckPoint);
	inline const class Scene* GetActiveScene() const { return myScenes[myActiveScene]; };
	std::vector<class Scene*> myScenes;
	std::vector<std::string> myScenePaths;
	UnityLoader myUnityLoader;
private:
	RenderManager myRenderManager;
	size_t myActiveScene = 0;
	PollingStation* myPollingStation;
};