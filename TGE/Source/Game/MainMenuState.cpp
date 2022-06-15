#include "stdafx.h"
#include "MainMenuState.h"
#include "StateStack.h"
#include "GameState.h"
#include "CreditsState.h"
#include "Button.h"
#include "InputMapperEvents.h"
#include "OptionsState.h"

#include <vector>
#include <tga2d/texture/TextureManager.h>
#include <tga2d/graphics/GraphicsEngine.h>
#include <tga2d/graphics/Camera.h>
#include <tga2d/drawers/SpriteDrawer.h>
#include <iostream>

#include "AudioManager.h"

MainMenuState::MainMenuState(StateStack& aStateStack, PollingStation* aPollingStation) 
	: 
	State(aStateStack, aPollingStation)
{
	SetPollingStation(aPollingStation);

	//Play music
	myPollingStation->myAudioManager->SetMusic(FSPRO::Event::music_meny);

	mySharedData.myTexture = Tga2D::Engine::GetInstance()->GetTextureManager().GetTexture(L"Sprites/UI/Menus/MainMenu/ui_mainMenu_background.dds");
	mySpriteInstance.myPosition = { 0.5f,0.5f };
	mySpriteInstance.mySizeMultiplier = { 1.f,1.81f };
	mySpriteInstance.myScaleSpritesWithAspectRatio = false;
	mySharedData.myTexture->CalculateTextureSize();
	mySharedData.mySamplerFilter = Tga2D::SamplerFilter::Bilinear;

	float xPos = 0.75f;
	float yPosition = 0.5f;
	float yIncrement = 0.1f;
	myButtons.push_back(Button(eButtonType::Start, { xPos, yPosition }));
	yPosition += yIncrement;
	myButtons.push_back(Button(eButtonType::Options, { xPos, yPosition }));
	yPosition += yIncrement;
	myButtons.push_back(Button(eButtonType::Credits, { xPos, yPosition }));
	yPosition += yIncrement;
	myButtons.push_back(Button(eButtonType::Exit, { xPos, yPosition }));

	myButtons[0].SetState(eState::Selected);
	mySelectedType = myButtons[0].GetType();

	myPollingStation->myInputMapper.get()->AddObserver(Input::eInputEvent::eMenuUp, this);
	myPollingStation->myInputMapper.get()->AddObserver(Input::eInputEvent::eMenuDown, this);
	myPollingStation->myInputMapper.get()->AddObserver(Input::eInputEvent::eSelect, this);
	myPollingStation->myInputMapper.get()->AddObserver(Input::eInputEvent::eDash, this);
}

MainMenuState::~MainMenuState()
{
	myPollingStation->myInputMapper.get()->RemoveObserver(Input::eInputEvent::eMenuUp, this);
	myPollingStation->myInputMapper.get()->RemoveObserver(Input::eInputEvent::eMenuDown, this);
	myPollingStation->myInputMapper.get()->RemoveObserver(Input::eInputEvent::eSelect, this);
	myPollingStation->myInputMapper.get()->RemoveObserver(Input::eInputEvent::eDash, this);
}

void MainMenuState::Init()
{}

int MainMenuState::Update(const float /*aDeltaTime*/)
{
	if (myIsActive == false)
		myIsActive = true;

	return myNumberOfPops;
}

void MainMenuState::RecieveEvent(const Input::eInputEvent aEvent, const float /*aValue*/)
{
	if (myIsActive)
	{
		switch (aEvent)
		{
		case Input::eInputEvent::eMenuDown:

			for (size_t i = 0; i < myButtons.size(); i++) //Cycle down
			{
				if (myButtons[i].GetState() == eState::Selected)
				{
					myButtons[i].SetState(eState::None);

					if (i < myButtons.size() - 1)
					{
						myButtons[i + 1].SetState(eState::Selected);
						mySelectedType = myButtons[i + 1].GetType();
					}
					else
					{
						myButtons[0].SetState(eState::Selected);
						mySelectedType = myButtons[0].GetType();
					}
					myPollingStation->myAudioManager->PlayEvent(FSPRO::Event::sfx_menu_menu_hoover);

					return;
				}
			}
			break;
		case Input::eInputEvent::eMenuUp:

			for (size_t i = 0; i < myButtons.size(); i++) //Cycle up
			{
				if (myButtons[i].GetState() == eState::Selected)
				{
					myButtons[i].SetState(eState::None);

					if (i > 0)
					{
						myButtons[i - 1].SetState(eState::Selected);
						mySelectedType = myButtons[i - 1].GetType();
					}
					else
					{
						myButtons[myButtons.size() - 1].SetState(eState::Selected);
						mySelectedType = myButtons[myButtons.size() - 1].GetType();
					}
					myPollingStation->myAudioManager->PlayEvent(FSPRO::Event::sfx_menu_menu_hoover);

					return;
				}
			}

			break;
		case Input::eInputEvent::eDash:
		case Input::eInputEvent::eSelect:
			InvokeButton(mySelectedType);
			myIsActive = false;
			break;
		default:
			break;
		}
	}
}

void MainMenuState::Render()
{
	Tga2D::Engine::GetInstance()->GetGraphicsEngine().GetSpriteDrawer().Draw(mySharedData, mySpriteInstance);

	for (Button b : myButtons)
	{
		b.Render();
	}
}

void MainMenuState::InvokeButton(eButtonType aType)
{
	if (myIsActive)
	{
		switch (aType)
		{
		case eButtonType::Start:
			myStateStack.PushState(new GameState(myStateStack, myPollingStation, "Tutorial"));
			myStateStack.InitState();
			break;
		case eButtonType::Options:
			myStateStack.PushState(new OptionsState(myStateStack, myPollingStation));
			break;
		case eButtonType::Credits:
			myStateStack.PushState(new CreditsState(myStateStack, myPollingStation));
			break;
		case eButtonType::Exit:
			PostQuitMessage(0);
			break;
		default:
			break;
		}
	}
	myIsActive = false;
}
