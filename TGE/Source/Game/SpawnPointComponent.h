#pragma once
#include "Component.h"

class SpawnPointComponent : public Component
{
public:
	SpawnPointComponent(std::string aName);
	~SpawnPointComponent();
	virtual void OnAwake() override;
	virtual void OnStart() override;
	virtual void OnUpdate(const float aDeltaTime) override;
private:
	std::string myName;
};