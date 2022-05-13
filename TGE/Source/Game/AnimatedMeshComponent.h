#pragma once
#include "Component.h"
#include <json.hpp>

class AnimatedMeshComponent : public Component
{
public:
	AnimatedMeshComponent() = default;
	AnimatedMeshComponent(const std::string& aMeshPath, const std::string& aAlbedoPath, const std::string& aNormalPath, const std::string& aReflectivePath, const nlohmann::json& aAnimationList, class Scene* aScene);
	
	void LoadMesh(const std::wstring& someFilePath, std::wstring* someTexturePaths = nullptr);
	virtual void OnAwake() override;
	virtual void OnStart() override;
	virtual void OnUpdate(const float aDeltaTime) override;
private:
	//std::unordered_map<std::string, size_t> myAnimations;
	bool myHasModel = false;
	size_t myModelHandle;
};