#include "stdafx.h"
#include "MeshComponent.h"
#include "Scene.h"

MeshComponent::MeshComponent(const std::string& aMeshPath, const std::string& aAlbedoPath, const std::string& aNormalPath, const std::string& aReflectivePath, Scene* aScene)
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
}

MeshComponent::~MeshComponent()
{
	if (myHasModel)
	{
		myScene->GetRenderObjectManager().DestroyModel(myModelHandle);
	}
}

void MeshComponent::OnAwake()
{
}

void MeshComponent::OnStart()
{

}

void MeshComponent::OnUpdate(const float /*aDeltaTime*/)
{
	if (myHasModel)
	{
		Tga2D::Transform scaledTransform = *myTransform;
		scaledTransform.SetScale(scaledTransform.GetScale() * 0.01f);
		myScene->GetRenderObjectManager().GetModel(myModelHandle)->SetTransform(scaledTransform);
	}
}

void MeshComponent::LoadMesh(const std::wstring& someFilePath, std::wstring* someTexturePaths)
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

	myModelHandle = myScene->GetRenderObjectManager().RegisterModel(someFilePath.c_str(), someTexturePaths);
	myHasModel = true;
}


Tga2D::ModelInstance* MeshComponent::GetMesh() const
{
	if (myHasModel)
	{
		return myScene->GetRenderObjectManager().GetModel(myModelHandle);
	}
	return nullptr;
}
