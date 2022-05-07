#include "stdafx.h"
#include "CreditsState.h"
#include "Button.h"

#include <vector>
#include <tga2d/texture/TextureManager.h>
#include <tga2d/graphics/GraphicsEngine.h>
#include <tga2d/graphics/Camera.h>
#include <tga2d/drawers/SpriteDrawer.h>

CreditsState::CreditsState(StateStack& aStateStack, PollingStation* aPollingStation)
    :
    State(aStateStack, aPollingStation)
{
	mySharedData.myTexture = Tga2D::Engine::GetInstance()->GetTextureManager().GetTexture(L"Sprites/UI/Menus/PauseMenu/ui_pauseMenu_background.dds");
	mySpriteInstance.myPosition = { 0.5f,0.5f };
	mySpriteInstance.mySizeMultiplier = { 2,1 };

	mySpriteInstance.myScaleSpritesWithAspectRatio = true;
	mySharedData.myTexture->CalculateTextureSize();
	mySharedData.mySamplerFilter = Tga2D::SamplerFilter::Bilinear;

	myBackButton = new Button(eButtonType::Back, { 0.5f,0.9f });
	myBackButton->SetState(eState::Selected);

	myPollingStation->myInputMapper.get()->AddObserver(Input::eInputEvent::eSelect, this);
	myPopInfo.myShouldPop = false;
}

CreditsState::~CreditsState()
{
	myPollingStation->myInputMapper.get()->RemoveObserver(Input::eInputEvent::eSelect, this);
	delete myBackButton;
}

void CreditsState::Init()
{}

PopInfo CreditsState::Update(const float /*aDeltaTime*/)
{
	myIsActive = true;
    return myPopInfo;
}

void CreditsState::RecieveEvent(const Input::eInputEvent aEvent, const float /*aValue*/)
{
	if (myIsActive)
	{
		switch (aEvent)
		{
		case Input::eInputEvent::eSelect:
			myPopInfo.myShouldPop = true;
			myPopInfo.myNumberOfPops = 1;
			break;
		default:
			break;
		}
	}
}

void CreditsState::Render()
{
	Tga2D::Engine::GetInstance()->GetGraphicsEngine().GetSpriteDrawer().Draw(mySharedData, mySpriteInstance);
	myBackButton->Render();
}