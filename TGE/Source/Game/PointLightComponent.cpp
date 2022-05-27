#include "stdafx.h"
#include "PointLightComponent.h"
#include "Scene.h"

PointLightComponent::PointLightComponent(const Tga2D::Color& aColor, const float anIntensity, const float someRange) : myColor(aColor), myIntensity(anIntensity), myRange(someRange)
{
}

PointLightComponent::~PointLightComponent()
{
	myScene->GetRenderObjectManager().DestroyPointLight(myLightHandle);
}

void PointLightComponent::OnAwake()
{
	myLightHandle = myScene->GetRenderObjectManager().RegisterPointLight(myColor, myIntensity, myRange);
}

void PointLightComponent::OnStart()
{
}

void PointLightComponent::OnUpdate(float aDeltaTime)
{
	aDeltaTime;
	myScene->GetRenderObjectManager().GetPointLight(myLightHandle)->SetTransform(*myTransform);
}
