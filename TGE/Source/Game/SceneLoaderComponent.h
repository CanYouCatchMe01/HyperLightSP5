#pragma once
#include "Component.h"

class SceneLoaderComponent : public Component
{
public:
	SceneLoaderComponent(std::string aScene, std::string aCheckpointName);
	void Load();
	void OnAwake() override;
	void OnStart() override;
	void OnUpdate(const float aDeltaTime) override;
private:
	std::string mySceneToLoad;
	std::string myCheckPointToLoad;
};