#include "stdafx.h"
#include "UIElement.h"
#include <vector>
#include <tga2d/texture/TextureManager.h>
#include <tga2d/graphics/GraphicsEngine.h>
#include <tga2d/graphics/Camera.h>
#include <tga2d/drawers/SpriteDrawer.h>
#include "PollingStation.h"
#include "Postmaster.h"

UIElement::UIElement(Tga2D::Vector2f aPosition, eElementType anElementType, PollingStation* aPollingStation)
{
	myPollingStation = aPollingStation;
	myStartSizeMultiplier = { .4f,.2f };
	float healthBarFillVariable1 = 3.5f / myStartSizeMultiplier.x;
	float healthBarFillVariable2 = (myStartSizeMultiplier.x) / (1.f / 3.f);
	mySpriteInstance.mySizeMultiplier = myStartSizeMultiplier;
	mySpriteInstance.myPosition = aPosition;
	myElementType = anElementType;

	switch (anElementType)
	{
	case eHealthKit:
		myTextures.push_back(Tga2D::Engine::GetInstance()->GetTextureManager().GetTexture(L"Sprites/UI/HUD/ui_hud_healingCharges_0.dds"));
		myTextures.push_back(Tga2D::Engine::GetInstance()->GetTextureManager().GetTexture(L"Sprites/UI/HUD/ui_hud_healingCharges_1.dds"));
		myTextures.push_back(Tga2D::Engine::GetInstance()->GetTextureManager().GetTexture(L"Sprites/UI/HUD/ui_hud_healingCharges_2.dds"));
		myTextures.push_back(Tga2D::Engine::GetInstance()->GetTextureManager().GetTexture(L"Sprites/UI/HUD/ui_hud_healingCharges_3.dds"));

		myPollingStation->myPostmaster->AddObserver(this, eMessageType::ePlayerPickedUpHealth);
		break;
	case eHealthBar:
		myTextures.push_back(Tga2D::Engine::GetInstance()->GetTextureManager().GetTexture(L"Sprites/UI/HUD/ui_hud_healthBar_life.dds"));
		myTextures.push_back(Tga2D::Engine::GetInstance()->GetTextureManager().GetTexture(L"Sprites/UI/HUD/ui_hud_healthBar_frame.dds"));
		mySharedDataOutline.myTexture = myTextures[1];
		mySpriteInstanceHealth.mySizeMultiplier = { myStartSizeMultiplier.x, myStartSizeMultiplier.y };
		mySpriteInstanceHealth.myPivot = { (myStartSizeMultiplier.x / healthBarFillVariable2), 0.5f };
		mySpriteInstanceHealth.myPosition = { aPosition.x- (mySpriteInstanceHealth.myPivot.x / healthBarFillVariable1),aPosition.y};

		myPollingStation->myPostmaster->AddObserver(this, eMessageType::ePlayerTookDMG);
		break;
	default:
		break;
	}

	myPollingStation->myPostmaster->AddObserver(this, eMessageType::ePlayerHealed);
	mySharedData.myTexture = myTextures[0];
}

UIElement::~UIElement()
{
	myPollingStation->myPostmaster->RemoveObserver(this, eMessageType::ePlayerHealed);

	switch (myElementType)
	{
	case eHealthKit:
		myPollingStation->myPostmaster->RemoveObserver(this, eMessageType::ePlayerPickedUpHealth);
		break;
	case eHealthBar:
		myPollingStation->myPostmaster->RemoveObserver(this, eMessageType::ePlayerTookDMG);
		break;
	default:
		break;
	}

}

void UIElement::Render()
{
	Tga2D::Engine::GetInstance()->GetGraphicsEngine().SetCamera(Tga2D::Camera());
	switch (myElementType)
	{
	case eHealthKit:
		Tga2D::Engine::GetInstance()->GetGraphicsEngine().GetSpriteDrawer().Draw(mySharedData, mySpriteInstance);
		break;
	case eHealthBar:
		Tga2D::Engine::GetInstance()->GetGraphicsEngine().GetSpriteDrawer().Draw(mySharedData, mySpriteInstanceHealth);
		Tga2D::Engine::GetInstance()->GetGraphicsEngine().GetSpriteDrawer().Draw(mySharedDataOutline, mySpriteInstance);
		break;
	default:
		break;
	}
}

void UIElement::RecieveMsg(const Message& aMsg)
{
	switch (aMsg.myMsg)
	{
	case eMessageType::ePlayerHealed:
		if (myNumberOfHealthKits > 0)
			myNumberOfHealthKits--;

		if (myElementType == eElementType::eHealthKit)
		{
			mySharedData.myTexture = myTextures[myNumberOfHealthKits];
		}

		if (myElementType == eElementType::eHealthBar)
		{

		}
		break;

	case eMessageType::ePlayerPickedUpHealth:
		myNumberOfHealthKits = aMsg.anIntValue;
		if (myElementType == eElementType::eHealthKit)
		{
			mySharedData.myTexture = myTextures[myNumberOfHealthKits];
		}
		break;

	case eMessageType::ePlayerTookDMG:
		if (myElementType == eElementType::eHealthBar && mySpriteInstanceHealth.mySizeMultiplier.x > 0)
		{
			mySpriteInstanceHealth.mySizeMultiplier = { (myStartSizeMultiplier.x * aMsg.aFloatValue), myStartSizeMultiplier.y };
		}
		if (mySpriteInstanceHealth.mySizeMultiplier.x < 0)
		{
			mySpriteInstanceHealth.mySizeMultiplier.x = 0;
		}
		break;

	default:
		break;
	}
}
