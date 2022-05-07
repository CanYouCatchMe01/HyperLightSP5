#pragma once
#include <vector>
#include <set>
#include <tga2d/model/ModelInstance.h>

namespace Tga2D
{
	class ForwardRenderer;
	class LightManager;
}

class RenderObjectManager
{
public:

	RenderObjectManager();
	void Render(Tga2D::ForwardRenderer& aForwardsRenderer);

	size_t RegisterModel(const wchar_t* aPath, std::wstring* someTexturePaths = nullptr);

	Tga2D::ModelInstance* GetModel(size_t aModelHandle);

	void DestroyModel(size_t aModel);

private:
	std::vector<Tga2D::ModelInstance> myModels;
	std::set<size_t> myEmptyModels;
};