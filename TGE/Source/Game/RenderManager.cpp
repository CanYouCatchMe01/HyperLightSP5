#include "stdafx.h"
#include <tga2d/light/LightManager.h>
#include <tga2d/graphics/DX11.h>
#include <tga2d/drawers/ModelDrawer.h>
#include <tga2d/graphics/FullscreenEffect.h>
#include "RenderManager.h"
#include "Scene.h"

RenderManager::RenderManager()
	: myForwardRenderer(Tga2D::Engine::GetInstance()->GetGraphicsEngine().GetForwardRenderer()),
	myGraphicsEngine(Tga2D::Engine::GetInstance()->GetGraphicsEngine()),
	myLightManager(Tga2D::Engine::GetInstance()->GetLightManager())
{
	myIntermediateDepth = Tga2D::DepthBuffer::Create(Tga2D::DX11::GetResolution(), DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT);
	myIntermediateTexture = Tga2D::RenderTarget::Create(Tga2D::DX11::GetResolution(), DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT);
}

void RenderManager::Render(Scene* aScene)
{
	myIntermediateTexture.Clear();
	myIntermediateDepth.Clear();

	Tga2D::DX11::RenderStateManager->ResetStates();

	myGraphicsEngine.SetCamera(aScene->myCamera);

	//Send data from CPU to GPU, CommonBuffer graphicsengine -> ShaderStructs.hlsli
	myGraphicsEngine.UpdateAndBindCommonBuffer();

	//Send light info to GPU
	myGraphicsEngine.UpdateAndBindLightBuffer();
	myForwardRenderer.SetEnvironmentMap(*aScene->myAmbientLight);

	myLightManager.ClearPointLights();

	/*for (auto& pointLight : aScene->myPointLights)
	{
		myLightManager.AddPointLight(*pointLight);
	}*/

	//aScene->GetRenderObjectManager().AddPointLights(myLightManager);

	Tga2D::DX11::RenderStateManager->SetBlendState(Tga2D::BlendState::Disabled);
	myIntermediateTexture.SetAsActiveTarget(&myIntermediateDepth);

	aScene->myRenderObjectManager.Render(myForwardRenderer);

	Tga2D::DX11::RenderStateManager->SetBlendState(Tga2D::BlendState::Disabled);
	Tga2D::DX11::BackBuffer->SetAsActiveTarget();

	//OBS. IF GRAPHICSDEBUG SET ANTIALIAZING SAMPLE COUNT TO 1 BEFORE
	myIntermediateTexture.SetAsResourceOnSlot(0);
	myGraphicsEngine.GetFullscreenEffectTonemap().Render();
	//OBS.

	aScene->GetCollisionManager().Render();
	//Reset camera and ambient light for 2D elements to render properly


}