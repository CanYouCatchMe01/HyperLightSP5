#pragma once
#include "Component.h"
#include <json.hpp>

class EmitterComponent : public Component
{
public:
	EmitterComponent(nlohmann::json& aParticleConfig, class Scene* aScene);
	~EmitterComponent();

	void OnAwake() override;
	void OnStart() override;
	void OnUpdate(const float aDeltaTime) override;
private:
	size_t myEmitterHandle;
};