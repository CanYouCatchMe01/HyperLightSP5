#include "stdafx.h"
#include "UIElement.h"


#include <tga2d/texture/TextureManager.h>
#include <tga2d/graphics/GraphicsEngine.h>
#include <tga2d/graphics/Camera.h>
#include <tga2d/drawers/SpriteDrawer.h>


UIElement::UIElement(Tga2D::Vector2f aPosition, Tga2D::Vector2f aSizeMultiplier, std::wstring aTexturePath)
{
	myStartSizeMultiplier = aSizeMultiplier;
	mySpriteInstance.mySizeMultiplier = myStartSizeMultiplier;
	mySpriteInstance.myPosition = aPosition;
	myTexture = Tga2D::Engine::GetInstance()->GetTextureManager().GetTexture(aTexturePath.c_str());
	mySharedData.myTexture = myTexture;
}

UIElement::~UIElement()
{}

void UIElement::Render()
{
	Tga2D::Engine::GetInstance()->GetGraphicsEngine().SetCamera(Tga2D::Camera());
	Tga2D::Engine::GetInstance()->GetGraphicsEngine().GetSpriteDrawer().Draw(mySharedData, mySpriteInstance);
}

void UIElement::SetAlpha(float aValue)
{
	mySpriteInstance.myColor = { 1,1,1,aValue };
}

void UIElement::ChangeTexture(std::wstring aTexturePath)
{
	myTexture = Tga2D::Engine::GetInstance()->GetTextureManager().GetTexture(aTexturePath.c_str());
	mySharedData.myTexture = myTexture;
}

void UIElement::ChangeSizeMultiplier(Tga2D::Vector2f aSizeMultiplier)
{
	mySpriteInstance.mySizeMultiplier = aSizeMultiplier;
}

void UIElement::SetPivot(Tga2D::Vector2f aPosition)
{
	mySpriteInstance.myPivot = aPosition;
}

void UIElement::SetPosition(Tga2D::Vector2f aPosition)
{
	mySpriteInstance.myPosition = aPosition;
}
