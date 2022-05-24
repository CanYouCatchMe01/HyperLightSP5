#pragma once
#include "Component.h"

class DirectionalLightComponent : public Component
{
public:
	DirectionalLightComponent(const Tga2D::Color& aColor, const float anIntensity);

	void OnAwake() override;
	void OnStart() override;
	void OnUpdate(float aDeltaTime) override;
};

