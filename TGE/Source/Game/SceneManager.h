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
	std::vector<class Scene*> myScenes;
#ifdef _DEBUG
	std::vector<std::string> myScenePaths;
#endif
	UnityLoader myUnityLoader;
private:
	RenderManager myRenderManager;



};