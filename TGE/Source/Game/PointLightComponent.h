#pragma once
#include "Component.h"

class PointLightComponent : public Component
{
public:
	PointLightComponent(const Tga2D::Color& aColor, const float anIntensity, const float someRange);
	~PointLightComponent();
	
	void OnAwake() override;
	void OnStart() override;
	void OnUpdate(float aDeltaTime) override;

private:
	Tga2D::Color myColor;
	float myIntensity;
	float myRange;

	size_t myLightHandle;
};

