#include "stdafx.h"
#include "RenderObjectManager.h"
#include <tga2d/model/ModelFactory.h>
#include <tga2d/graphics/ForwardRenderer.h>
#include <tga2d/graphics/RenderStateManager.h>
#include <tga2d/graphics/DX11.h>
#include <functional>
#include <utility>
#include <tga2d/drawers/SpriteDrawer.h>

RenderObjectManager::RenderObjectManager()
{
}

void RenderObjectManager::Render(Tga2D::ForwardRenderer& aForwardsRenderer)
{
	for (size_t i = 0; i != myModels.size(); ++i)
	{
		if (myEmptyModels.count(i))
			continue;

		aForwardsRenderer.Render(myModels[i]);
	}

	for (size_t i = 0; i != myAnimatedModels.size(); ++i)
	{
		if (myEmptyAnimatedModels.count(i))
			continue;

		aForwardsRenderer.Render(myAnimatedModels[i]);
	}

}

size_t RenderObjectManager::RegisterModel(const wchar_t* aPath, std::wstring* someTexturePaths)
{
	if (myEmptyModels.size())
	{
		size_t handle = *std::prev(myEmptyModels.end());
		myEmptyModels.erase(std::prev(myEmptyModels.end()));
		myModels[handle] = Tga2D::ModelFactory::GetInstance().GetModel(aPath);
		return handle;
	}

	myModels.push_back(Tga2D::ModelFactory::GetInstance().GetModel(aPath, someTexturePaths));
	return myModels.size() - 1;
}

size_t RenderObjectManager::RegisterAnimatedModel(const wchar_t* aPath, std::wstring* someTexturePaths)
{
	if (myEmptyAnimatedModels.size())
	{
		size_t handle = *std::prev(myEmptyAnimatedModels.end());
		myEmptyAnimatedModels.erase(std::prev(myEmptyAnimatedModels.end()));
		myAnimatedModels[handle] = Tga2D::ModelFactory::GetInstance().GetAnimatedModel(aPath);
		return handle;
	}

	myAnimatedModels.push_back(Tga2D::ModelFactory::GetInstance().GetAnimatedModel(aPath, someTexturePaths));
	return myAnimatedModels.size() - 1;
}


Tga2D::ModelInstance* RenderObjectManager::GetModel(size_t aModelHandle)
{
	return &myModels[aModelHandle];
}

Tga2D::AnimatedModelInstance* RenderObjectManager::GetAnimatedModel(size_t aModelHandle)
{
	return &myAnimatedModels[aModelHandle];
}

void RenderObjectManager::DestroyModel(size_t aModel)
{
	myEmptyModels.insert(aModel);
}

void RenderObjectManager::DestroyAnimatedModel(size_t aModel)
{
	myEmptyAnimatedModels.insert(aModel);
}
