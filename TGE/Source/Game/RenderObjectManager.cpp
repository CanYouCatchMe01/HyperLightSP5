#include "stdafx.h"
#include "RenderObjectManager.h"
#include <tga2d/model/ModelFactory.h>
#include <tga2d/graphics/ForwardRenderer.h>
#include <tga2d/graphics/RenderStateManager.h>
#include <tga2d/graphics/DX11.h>
#include <functional>
#include <utility>
#include <tga2d/drawers/SpriteDrawer.h>
#include "tga2d/light/LightManager.h"
#include <tga2d/graphics/GraphicsEngine.h>
#include "GameObject.h"

RenderObjectManager::RenderObjectManager()
{
}

void RenderObjectManager::Render(Tga2D::ForwardRenderer& aForwardsRenderer)
{
#ifdef _DEBUG
	static float angle = 1.2f;
	static float distance = 100.f;
	ImGui::Begin("Culling");
	ImGui::DragFloat("Cull Angle", &angle, 0.01f, 0.f, 3.14f);
	ImGui::DragFloat("Cull Distance", &distance, 10.f, 0.f, 1000000.f);
	ImGui::End();
#else
	const float angle = 1.2f;
	const float distance = 100.0f;
#endif // DEBUG

	const Tga2D::Camera& camera = Tga2D::Engine::GetInstance()->GetGraphicsEngine().GetCamera();
	const Tga2D::Vector3f cameraPos = camera.GetTransform().GetPosition();
	const Tga2D::Vector3f forward = camera.GetTransform().GetMatrix().GetForward().GetNormalized();

	for (size_t i = 0; i != myModels.size(); i++)
	{
		if (myEmptyModels.count(i))
			continue;

		float modelRadius = myModels[i].GetModel()->GetMeshDataList()[0].myBounds.Radius;
		const Tga2D::Vector3f modelPos = myModels[i].GetTransform().GetPosition();


		Tga2D::Vector3f v = modelPos - cameraPos;
		const float lenSq = v.Dot(v);
		const float v1Len = v.Dot(forward);
		const float distanceClosestPoint = cos(angle) * sqrt(lenSq - v1Len * v1Len) - v1Len * sin(angle);

		const bool angleCull = distanceClosestPoint < modelRadius;
		const bool distanceCull = v.Length() < distance;

		if (angleCull || distanceCull)
		{
			aForwardsRenderer.Render(myModels[i]);
		}
	}

	for (size_t i = 0; i != myAnimatedModels.size(); ++i)
	{
		if (myEmptyAnimatedModels.count(i))
			continue;

		const auto& c = myAnimatedModels[i];
		Tga2D::Transform t = c.myParent->GetTransform();
		t.SetScale(t.GetScale() * 0.01f);

		myAnimatedModels[i].myAnimatedModel.SetTransform(t);
		aForwardsRenderer.Render(c.myAnimatedModel);
	}

	Tga2D::DX11::RenderStateManager->SetDepthStencilState(Tga2D::DepthStencilState::ReadOnly); // Render all transparent objects after here

	for (size_t i = 0; i < myEmitters.Size(); i++)
		myEmitters[i].Render();

	Tga2D::DX11::RenderStateManager->SetDepthStencilState(Tga2D::DepthStencilState::Write); // Render all transparent objects before here
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

size_t RenderObjectManager::RegisterAnimatedModel(const wchar_t* aPath, GameObject* aParent, std::wstring* someTexturePaths)
{
	AnimatedModelContatiner c;
	c.myAnimatedModel = Tga2D::ModelFactory::GetInstance().GetAnimatedModel(aPath, someTexturePaths);
	c.myParent = aParent;

	if (myEmptyAnimatedModels.size())
	{
		size_t handle = *std::prev(myEmptyAnimatedModels.end());
		myEmptyAnimatedModels.erase(std::prev(myEmptyAnimatedModels.end()));
		myAnimatedModels[handle] = c;
		return handle;
	}

	myAnimatedModels.push_back(c);
	return myAnimatedModels.size() - 1;
}

size_t RenderObjectManager::RegisterPointLight(const Tga2D::Color& aColor, const float anIntensity, float someRange)
{
	auto light = Tga2D::PointLight(
		Tga2D::Transform{
			Tga2D::Vector3f(0, 0, 0),
			Tga2D::Rotator::Zero
		},
		aColor,
		anIntensity, someRange
	);

	if (myEmptyPointLights.size())
	{
		size_t handle = *std::prev(myEmptyPointLights.end());
		myEmptyPointLights.erase(std::prev(myEmptyPointLights.end()));
		myPointLights[handle] = light;
		return handle;
	}

	myPointLights.push_back(light);
	return myPointLights.size() - 1;
}

Tga2D::PointLight* RenderObjectManager::GetPointLight(size_t aPointLight)
{
	return &myPointLights[aPointLight];
}

void RenderObjectManager::DestroyPointLight(size_t aPointLight)
{
	myEmptyPointLights.insert(aPointLight);
}

void RenderObjectManager::AddPointLights(Tga2D::LightManager& aLightManager)
{
	for (size_t i = 0; i != myPointLights.size(); ++i)
	{
		if (myEmptyPointLights.count(i))
			continue;

		aLightManager.AddPointLight(myPointLights[i]);
	}
}

size_t RenderObjectManager::RegisterEmitter(nlohmann::json& aParticleConfig)
{
	myEmitters.Insert(Emitter(aParticleConfig), myNextEmitter);
	return myNextEmitter++;
}

void RenderObjectManager::DestroyEmitter(size_t aEmitterHandle)
{
	myEmitters.Remove(aEmitterHandle);
}


Tga2D::ModelInstance* RenderObjectManager::GetModel(size_t aModelHandle)
{
	return &myModels[aModelHandle];
}

Tga2D::AnimatedModelInstance* RenderObjectManager::GetAnimatedModel(size_t aModelHandle)
{
	return &myAnimatedModels[aModelHandle].myAnimatedModel;
}

void RenderObjectManager::DestroyModel(size_t aModel)
{
	myEmptyModels.insert(aModel);
}

void RenderObjectManager::DestroyAnimatedModel(size_t aModel)
{
	myEmptyAnimatedModels.insert(aModel);
}
