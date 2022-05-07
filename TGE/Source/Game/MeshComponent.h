#pragma once
#include "Component.h"

namespace Tga2D
{
	class ModelInstance;
}

class MeshComponent : public Component
{
public:
	MeshComponent() = default;
	MeshComponent(const std::string& aMeshPath, const std::string& aAlbedoPath, const std::string& aNormalPath, const std::string& aReflectivePath, class Scene* aScene);
	~MeshComponent();
	void OnAwake() override;
	void OnStart() override;
	void OnUpdate(const float aDeltaTime) override;
	void LoadMesh(const std::wstring& someFilePath, std::wstring* someTexturePaths = nullptr);
	Tga2D::ModelInstance* GetMesh() const;
private:
	bool myHasModel = false;
	size_t myModelHandle;
};