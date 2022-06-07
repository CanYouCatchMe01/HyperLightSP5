#pragma once
#include "Component.h"
#include "json.hpp"
#include "Timer.h"

class CassetTapeShowerComponent : public Component
{
public:
	CassetTapeShowerComponent(const float aWaitSeconds, const nlohmann::json& aSpawnObject);

	void OnAwake() override;
	void OnStart() override;
	void OnUpdate(float aDeltaTime) override;

private:
	float myWaitSeconds;
	nlohmann::json mySpawnObject;
	Timer myTimer;
};

