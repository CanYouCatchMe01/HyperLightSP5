#pragma once
#include "Component.h"
#include "PlayerComponent.h"
#include <cstdint>
class CheckPointComponent : public Component
{
public:
	CheckPointComponent(std::string aSceneName, std::string aCheckPointName);
	~CheckPointComponent();
	void OnAwake() override;
	void OnStart() override;
	void OnUpdate(const float aDeltaTime) override;
	void Save();
	void Load();

private:
	PlayerData myPlayerData;
	std::string mySceneName;
	std::string myCheckPointName;
};