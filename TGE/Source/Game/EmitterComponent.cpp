#include "stdafx.h"
#include "EmitterComponent.h"
#include "Scene.h"

EmitterComponent::EmitterComponent(nlohmann::json& aParticleConfig, Scene* aScene)
{
	myEmitterHandle = aScene->GetRenderObjectManager().RegisterEmitter(aParticleConfig);
}

EmitterComponent::~EmitterComponent()
{
	myScene->GetRenderObjectManager().DestroyEmitter(myEmitterHandle);
}

void EmitterComponent::OnAwake()
{
}

void EmitterComponent::OnStart()
{
}

void EmitterComponent::OnUpdate(const float aDeltaTime)
{
	if (!myStatus) return;
	Emitter* emitter = myScene->GetRenderObjectManager().GetEmitter(myEmitterHandle);
	emitter->SetTransform(*myTransform);
	emitter->Update(aDeltaTime);
}
