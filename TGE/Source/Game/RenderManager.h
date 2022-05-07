#pragma once
#include <tga2d/graphics/GraphicsEngine.h>
#include <tga2d/graphics/ForwardRenderer.h>
#include <tga2d/light/LightManager.h>
#include <tga2d/graphics/DepthBuffer.h>
#include <tga2d/graphics/RenderTarget.h>

class RenderManager
{
public:
	RenderManager();
	void Render(class Scene* aScene);
private:
	Tga2D::DepthBuffer myIntermediateDepth;
	Tga2D::RenderTarget myIntermediateTexture;

	Tga2D::ForwardRenderer& myForwardRenderer;
	Tga2D::LightManager& myLightManager;
	Tga2D::GraphicsEngine& myGraphicsEngine;
};