#include "stdafx.h"
#include "BobbingComponent.h"

BobbingComponent::BobbingComponent()
{
}

BobbingComponent::~BobbingComponent()
{
}

void BobbingComponent::OnAwake()
{
	myStartY = myTransform->GetPosition().y;
}

void BobbingComponent::OnStart()
{
}

void BobbingComponent::OnUpdate(float aDeltaTime)
{
	myBobbingVariable += aDeltaTime;
	Tga2D::Vector3f pos = myTransform->GetPosition();
	pos.y = sin(myBobbingVariable) * 0.25f + myStartY;
	myTransform->SetPosition(pos);
	Tga2D::Rotator rot = myTransform->GetRotation();
	rot.y += aDeltaTime * 25;
	myTransform->SetRotation(rot);
}
