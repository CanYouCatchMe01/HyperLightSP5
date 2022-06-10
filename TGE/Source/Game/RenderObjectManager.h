#pragma once
#include <vector>
#include <set>
#include <tga2d/model/ModelInstance.h>
#include <tga2d/model/AnimatedModelInstance.h>
#include <tga2d/graphics/PointLight.h>
#include "SparseSet.hpp"
#include "Particles.h"
#include <json.hpp>

namespace Tga2D
{
	class ForwardRenderer;
	class LightManager;
}

class GameObject;

struct AnimatedModelContatiner
{
	GameObject* myParent;
	Tga2D::AnimatedModelInstance myAnimatedModel;
};

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
	size_t RegisterAnimatedModel(const wchar_t* aPath, GameObject* aParent, std::wstring* someTexturePaths = nullptr);
	Tga2D::AnimatedModelInstance* GetAnimatedModel(size_t aModelHandle);
	void DestroyAnimatedModel(size_t aModel);

	//PointLight
	size_t RegisterPointLight(const Tga2D::Color& aColor, const float anIntensity, float someRange);
	Tga2D::PointLight* GetPointLight(size_t aPointLight);
	void DestroyPointLight(size_t aPointLight);
	void AddPointLights(Tga2D::LightManager& aLightManager); //Sending Pointlights to the LightManager

	size_t RegisterEmitter(nlohmann::json& aParticleConfig);
	inline Emitter* GetEmitter(size_t aEmitterHandle) { return &myEmitters.Get(aEmitterHandle); };
	void DestroyEmitter(size_t aEmitterHandle);
	
private:
	//Model
	std::vector<Tga2D::ModelInstance> myModels;
	std::set<size_t> myEmptyModels;

	//Animated Model
	std::vector<AnimatedModelContatiner> myAnimatedModels;
	std::set<size_t> myEmptyAnimatedModels;

	//PointLight
	std::vector<Tga2D::PointLight> myPointLights;
	std::set<size_t> myEmptyPointLights;
	
	SparseSet<Emitter> myEmitters;
	size_t myNextEmitter = 0;
};