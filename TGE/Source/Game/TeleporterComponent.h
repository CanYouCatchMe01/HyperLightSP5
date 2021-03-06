#pragma once
#include "Component.h"

class TeleporterComponent : public Component
{
public:
	TeleporterComponent(std::string aScene, std::string aCheckpointName);
	~TeleporterComponent();
	void Load();
	void OnAwake() override;
	void OnStart() override;
	void OnUpdate(const float aDeltaTime) override;
private:
	std::string mySceneToLoad;
	std::string myCheckPointToLoad;
};