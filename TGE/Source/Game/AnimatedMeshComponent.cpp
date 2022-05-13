#include "stdafx.h"
#include "AnimatedMeshComponent.h"
#include "Scene.h"

AnimatedMeshComponent::AnimatedMeshComponent(const std::string& aMeshPath, const std::string& aAlbedoPath, const std::string& aNormalPath, const std::string& aReflectivePath, const nlohmann::json& aAnimationList, class Scene* aScene)
{
	myHasModel = false;
	myScene = aScene;

	

	std::wstring textures[3] =
	{
		std::wstring(aAlbedoPath.begin(), aAlbedoPath.end()),
		std::wstring(aNormalPath.begin(), aNormalPath.end()),
		std::wstring(aReflectivePath.begin(), aReflectivePath.end())
	};

	LoadMesh(std::wstring(aMeshPath.begin(), aMeshPath.end()), textures);

	std::vector <Tga2D::AnimationImportDefinition> animations;
	//size_t index = 0;

	for (auto& j : aAnimationList)
	{
		Tga2D::AnimationImportDefinition def;
		std::string tempPath = j["path"];
		std::wstring path(tempPath.begin(), tempPath.end());

		def.Path = path;
		def.ShouldLoop = true; // Temporary
		animations.push_back(def);
	}

	aScene->GetRenderObjectManager().GetAnimatedModel(myModelHandle)->InitAnimations(animations);
	aScene->GetRenderObjectManager().GetAnimatedModel(myModelHandle)->PlayAnimation(0);
}

void AnimatedMeshComponent::LoadMesh(const std::wstring& someFilePath, std::wstring* someTexturePaths)
{
#pragma warning(disable : 4244)
	struct stat buffer;
	std::string s = std::string(someFilePath.begin(), someFilePath.end());
	bool hasFile = (stat(s.c_str(), &buffer) == 0);
	if (!hasFile)
	{
		ERROR_PRINT(("Model " + s + std::string(" does not exist")).c_str());
		return;
	}
#pragma warning(default : 4244)

	if (myHasModel)
		myScene->GetRenderObjectManager().DestroyModel(myModelHandle);

	myModelHandle = myScene->GetRenderObjectManager().RegisterAnimatedModel(someFilePath.c_str(), someTexturePaths);
	myHasModel = true;
}

void AnimatedMeshComponent::OnAwake()
{
}

void AnimatedMeshComponent::OnStart()
{
}

void AnimatedMeshComponent::OnUpdate(const float aDeltaTime)
{
	if (myHasModel)
	{
		Tga2D::AnimatedModelInstance* model = myScene->GetRenderObjectManager().GetAnimatedModel(myModelHandle);
		Tga2D::Transform scaled = *myTransform;
		scaled.SetScale(scaled.GetScale() * 0.01f);
		model->SetTransform(scaled);
		model->Update(aDeltaTime);
	}
	
}
