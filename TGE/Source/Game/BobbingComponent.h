#pragma once
#include "Component.h"

class BobbingComponent : public Component
{
public:
	BobbingComponent();
	~BobbingComponent();
	void OnAwake() override;
	void OnStart() override;
	void OnUpdate(float aDeltaTime) override;
private:
	float myBobbingVariable = 0;
	float myStartY;
};