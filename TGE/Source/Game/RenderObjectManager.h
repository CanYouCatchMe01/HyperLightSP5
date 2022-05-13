#pragma once
#include <vector>
#include <set>
#include <tga2d/model/ModelInstance.h>
#include <tga2d/model/AnimatedModelInstance.h>

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
	size_t RegisterAnimatedModel(const wchar_t* aPath, std::wstring* someTexturePaths = nullptr);

	Tga2D::ModelInstance* GetModel(size_t aModelHandle);
	Tga2D::AnimatedModelInstance* GetAnimatedModel(size_t aModelHandle);

	void DestroyModel(size_t aModel);
	void DestroyAnimatedModel(size_t aModel);

private:
	std::vector<Tga2D::ModelInstance> myModels;
	std::vector<Tga2D::AnimatedModelInstance> myAnimatedModels;

	std::set<size_t> myEmptyModels;
	std::set<size_t> myEmptyAnimatedModels;
};