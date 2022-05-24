#pragma once
#include "Component.h"

class PointLightComponent : public Component
{
public:
	PointLightComponent(const Tga2D::Color& aColor, const float anIntensity, const float someRange);
	
	void OnAwake() override;
	void OnStart() override;
	void OnUpdate(float aDeltaTime) override;
};

