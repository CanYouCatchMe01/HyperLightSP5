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
	inline void SetEmitterStatus(bool aStatus) { myStatus = aStatus; }
private:
	bool myStatus = true;
	size_t myEmitterHandle;
};