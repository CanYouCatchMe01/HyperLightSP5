#pragma once
#include "Component.h"

class TeleporterComponent : public Component
{
public:
	TeleporterComponent(std::string aScene);
	void Load();
	void Activate();
	void OnAwake() override;
	void OnStart() override;
	void OnUpdate(const float aDeltaTime) override;
private:
	bool myIsActive = false;
	std::string mySceneName;
};