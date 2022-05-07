#include "stdafx.h"
#include "PauseMenuState.h"
#include "StateStack.h"
#include "OptionsState.h"
#include "Button.h"

#include <vector>
#include <tga2d/texture/TextureManager.h>
#include <tga2d/graphics/GraphicsEngine.h>
#include <tga2d/graphics/Camera.h>
#include <tga2d/drawers/SpriteDrawer.h>


PauseMenuState::PauseMenuState(StateStack& aStateStack, PollingStation* aPollingStation)
	:
	State(aStateStack, aPollingStation)
{
	mySharedData.myTexture = Tga2D::Engine::GetInstance()->GetTextureManager().GetTexture(L"Sprites/UI/Menus/PauseMenu/ui_pauseMenu_background.dds");
	mySpriteInstance.myPosition = { 0.5f,0.5f};
	mySpriteInstance.mySizeMultiplier={2,1};

	mySpriteInstance.myScaleSpritesWithAspectRatio = true;
	mySharedData.myTexture->CalculateTextureSize();
	mySharedData.mySamplerFilter = Tga2D::SamplerFilter::Bilinear;


	float yPosition = mySpriteInstance.myPosition.myY;
	float yIncrement = 0.14f;

	myButtons.push_back(new Button(eButtonType::Resume, { mySpriteInstance.myPosition.x, yPosition }));
	yPosition += yIncrement;
	myButtons.push_back(new Button(eButtonType::Options, { mySpriteInstance.myPosition.x, yPosition }));
	yPosition += yIncrement;
	myButtons.push_back(new Button(eButtonType::ExitToMain, { mySpriteInstance.myPosition.x, yPosition }));

	myButtons[0]->SetState(eState::Selected);
	mySelectedType = myButtons[0]->GetType();

	myPollingStation->myInputMapper.get()->AddObserver(Input::eInputEvent::eMenuUp, this);
	myPollingStation->myInputMapper.get()->AddObserver(Input::eInputEvent::eMenuDown, this);
	myPollingStation->myInputMapper.get()->AddObserver(Input::eInputEvent::eSelect, this);

	myPopInfo.myShouldPop = false;
}

PauseMenuState::~PauseMenuState()
{
	myPollingStation->myInputMapper.get()->RemoveObserver(Input::eInputEvent::eMenuUp, this);
	myPollingStation->myInputMapper.get()->RemoveObserver(Input::eInputEvent::eMenuDown, this);
	myPollingStation->myInputMapper.get()->RemoveObserver(Input::eInputEvent::eSelect, this);

	for (Button* b : myButtons)
	{
		delete b;
	}
}

PopInfo PauseMenuState::Update(const float /*aDeltaTime*/)
{
	myIsActive = true;
	return myPopInfo;
}

void PauseMenuState::Init()
{}

void PauseMenuState::Render()
{
	myStateStack.RenderUnderlyingState();

	Tga2D::Engine::GetInstance()->GetGraphicsEngine().SetCamera(Tga2D::Camera());
	Tga2D::Engine::GetInstance()->GetGraphicsEngine().GetSpriteDrawer().Draw(mySharedData, mySpriteInstance);
	for (Button* b : myButtons)
	{
		b->Render();
	}
}

void PauseMenuState::InvokeButton(eButtonType aType)
{
	switch (aType)
	{
	case eButtonType::Resume:
		myPopInfo.myNumberOfPops = 1;
		myPopInfo.myShouldPop = true;
		break;
	case eButtonType::Options:
		myStateStack.PushState(new OptionsState(myStateStack, myPollingStation));
		myIsActive = false;
		break;
	case eButtonType::ExitToMain:
		myPopInfo.myNumberOfPops = 2;
		myPopInfo.myShouldPop = true;
		Tga2D::Engine::GetInstance()->GetGraphicsEngine().SetCamera(Tga2D::Camera());
		break;
	case eButtonType::Exit:
		break;
	default:
		break;
	}
}

void PauseMenuState::RecieveEvent(const Input::eInputEvent aEvent, const float /*aValue*/)
{
	if (myIsActive)
	{
		switch (aEvent)
		{
		case Input::eInputEvent::eMenuDown:

			for (size_t i = 0; i < myButtons.size(); i++) //Cycle down
			{
				if (myButtons[i]->GetState() == eState::Selected)
				{
					myButtons[i]->SetState(eState::None);

					if (i < myButtons.size() - 1)
					{
						myButtons[i + 1]->SetState(eState::Selected);
						mySelectedType = myButtons[i + 1]->GetType();
					}
					else
					{
						myButtons[0]->SetState(eState::Selected);
						mySelectedType = myButtons[0]->GetType();
					}
					return;

				}
			}
			break;
		case Input::eInputEvent::eMenuUp:

			for (size_t i = 0; i < myButtons.size(); i++) //Cycle up
			{
				if (myButtons[i]->GetState() == eState::Selected)
				{
					myButtons[i]->SetState(eState::None);

					if (i > 0)
					{
						myButtons[i - 1]->SetState(eState::Selected);
						mySelectedType = myButtons[i - 1]->GetType();
					}
					else
					{
						myButtons[myButtons.size() - 1]->SetState(eState::Selected);
						mySelectedType = myButtons[myButtons.size() - 1]->GetType();
					}
					return;

				}
			}

			break;
		case Input::eInputEvent::eSelect:
			InvokeButton(mySelectedType);
			break;
		default:
			break;
		}
	}
}
