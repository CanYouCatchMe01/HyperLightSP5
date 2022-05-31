#pragma once
#include "Component.h"

class TeleportActivator : public Component
{
public:
	TeleportActivator();
	~TeleportActivator();
	void Activate();

	void OnAwake() override;
	void OnStart() override;
	void OnUpdate(const float aDeltaTime) override;
private:
	bool myIsActive = false;
	std::string mySceneName;
};