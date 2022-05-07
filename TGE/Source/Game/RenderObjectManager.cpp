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


Tga2D::ModelInstance* RenderObjectManager::GetModel(size_t aModelHandle)
{
	return &myModels[aModelHandle];
}

void RenderObjectManager::DestroyModel(size_t aModel)
{
	myEmptyModels.insert(aModel);
}