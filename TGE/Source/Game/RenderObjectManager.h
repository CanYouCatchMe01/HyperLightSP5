#pragma once
#include <vector>
#include <set>
#include <tga2d/model/ModelInstance.h>
#include <tga2d/model/AnimatedModelInstance.h>
#include <tga2d/graphics/PointLight.h>

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

	//Model
	size_t RegisterModel(const wchar_t* aPath, std::wstring* someTexturePaths = nullptr);
	Tga2D::ModelInstance* GetModel(size_t aModelHandle);
	void DestroyModel(size_t aModel);
	
	//Animated Model
	size_t RegisterAnimatedModel(const wchar_t* aPath, std::wstring* someTexturePaths = nullptr);
	Tga2D::AnimatedModelInstance* GetAnimatedModel(size_t aModelHandle);
	void DestroyAnimatedModel(size_t aModel);

	//PointLight
	size_t RegisterPointLight(const Tga2D::Color& aColor, const float anIntensity, float someRange);
	Tga2D::PointLight* GetPointLight(size_t aPointLight);
	void DestroyPointLight(size_t aPointLight);
	void AddPointLights(Tga2D::LightManager& aLightManager); //Sending Pointlights to the LightManager
	
private:
	//Model
	std::vector<Tga2D::ModelInstance> myModels;
	std::set<size_t> myEmptyModels;

	//Animated Model
	std::vector<Tga2D::AnimatedModelInstance> myAnimatedModels;
	std::set<size_t> myEmptyAnimatedModels;

	//PointLight
	std::vector<Tga2D::PointLight> myPointLights;
	std::set<size_t> myEmptyPointLights;
};