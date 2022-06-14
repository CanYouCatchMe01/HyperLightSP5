#pragma once
#include "Component.h"
#include <json.hpp>
#include "DebugObserver.h"
#include "AnimationController.h"

namespace Tga2D
{
	class AnimatedModelInstance;
}

class AnimatedMeshComponent : public Component
{
public:
	AnimatedMeshComponent(const std::string& aMeshPath, const std::string& aAlbedoPath, const std::string& aNormalPath, const std::string& aReflectivePath, const nlohmann::json& aAnimationList, class Scene* aScene, class GameObject* aGameObject);
	~AnimatedMeshComponent();
	
	void PlayAnimation(std::string aName);
	inline AnimationController::Transition* AddTransition(std::string aName, std::function<bool()> aCondition, float aSpeed = 1.0f) { return myAnimationController.AddTransition(aName, aCondition, aSpeed); }
	virtual void OnAwake() override;
	virtual void OnStart() override;
	virtual void OnUpdate(const float aDeltaTime) override;
	void LoadMesh(const std::wstring& someFilePath, std::wstring* someTexturePaths = nullptr);

#ifdef _DEBUG
	void DebugUpdate() override;
#endif // _DEBUG

private:
	void LoadMesh(const std::wstring& someFilePath, GameObject* aGameObject, std::wstring* someTexturePaths = nullptr);
	AnimationController myAnimationController;
	Tga2D::AnimatedModelInstance* GetModel();
	std::vector<std::pair<std::string, size_t>> myAnimations;
	bool myHasModel = false;
	size_t myModelHandle;
};