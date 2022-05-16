#include "stdafx.h"
#include "OptionsState.h"
#include "Button.h"
#include "StateStack.h"
#include "tga2d/engine.h"
#include "PollingStation.h"
#include "AudioManager.h"
#include <vector>
#include <tga2d/texture/TextureManager.h>
#include <tga2d/graphics/GraphicsEngine.h>
#include <tga2d/graphics/Camera.h>
#include <tga2d/drawers/SpriteDrawer.h>

OptionsState::OptionsState(StateStack& aStateStack, PollingStation* aPollingStation)
	:
	State(aStateStack, aPollingStation)
{
	mySharedData.myTexture = Tga2D::Engine::GetInstance()->GetTextureManager().GetTexture(L"Sprites/UI/Menus/OptionsMenu/ui_optionsMenu_background.dds");
	mySpriteInstance.myPosition = { 0.5f,0.5f };
	mySpriteInstance.myScaleSpritesWithAspectRatio = false;
	float leftArrowX = 0.38f;
	float rightArrowX = 0.62f;
	float yPosition = 0.35f;
	float yIncrement = 0.12f;

	myButtons.push_back(Button(eButtonType::FullScreen, { 0.5f, yPosition }));

	int buttonIndex = (int)eButtonType::Resolution;
	for (size_t i = 0; i < 4; i++)
	{
		yPosition += yIncrement;
		myButtons.push_back(Button((eButtonType)buttonIndex, { 0.5f, yPosition }));
		myArrows.push_back(Button(eButtonType::ArrowLeft, { leftArrowX,yPosition }));
		myArrows.push_back(Button(eButtonType::ArrowRight, { rightArrowX,yPosition }));
		buttonIndex++;
	}
	yPosition += yIncrement;
	myButtons.push_back(Button(eButtonType::Back,		{ 0.5f, yPosition }));

	myButtons[0].SetState(eState::Selected);
	mySelectedButton = myButtons[0].GetType();
	mySelectedButtonIndex = 0;

	myPollingStation->myInputMapper.get()->AddObserver(Input::eInputEvent::eMenuUp, this);
	myPollingStation->myInputMapper.get()->AddObserver(Input::eInputEvent::eMenuDown, this);
	myPollingStation->myInputMapper.get()->AddObserver(Input::eInputEvent::eMenuLeft, this);
	myPollingStation->myInputMapper.get()->AddObserver(Input::eInputEvent::eMenuRight, this);
	myPollingStation->myInputMapper.get()->AddObserver(Input::eInputEvent::eSelect, this);
	myPollingStation->myInputMapper.get()->AddObserver(Input::eInputEvent::eReleaseArrowLeft, this);
	myPollingStation->myInputMapper.get()->AddObserver(Input::eInputEvent::eReleaseArrowRight, this);

	myPopInfo.myShouldPop = false;

	myScreenResolutions.push_back({ 960, 540 });
	myScreenResolutions.push_back({ 1280,720 });
	myScreenResolutions.push_back({ 1920,1080 });
	myScreenResolutions.push_back({ 2560,1440 });

	myScreenResIndex = 1;
	myButtons[1].SetText(std::to_string(myScreenResolutions[myScreenResIndex].x) + "x" + std::to_string(myScreenResolutions[myScreenResIndex].y));
	myButtons[2].SetText(std::to_string((int)(myPollingStation->myAudioManager.get()->GetVolume(Channels::Master) * 100)));
	myButtons[3].SetText(std::to_string((int)(myPollingStation->myAudioManager.get()->GetVolume(Channels::Music) * 100)));
	myButtons[4].SetText(std::to_string((int)(myPollingStation->myAudioManager.get()->GetVolume(Channels::SFX) * 100)));
	myVolumeChangeSpeed = .1f;
}

OptionsState::~OptionsState()
{
	myPollingStation->myInputMapper.get()->RemoveObserver(Input::eInputEvent::eMenuUp, this);
	myPollingStation->myInputMapper.get()->RemoveObserver(Input::eInputEvent::eMenuDown, this);
	myPollingStation->myInputMapper.get()->RemoveObserver(Input::eInputEvent::eMenuLeft, this);
	myPollingStation->myInputMapper.get()->RemoveObserver(Input::eInputEvent::eMenuRight, this);
	myPollingStation->myInputMapper.get()->RemoveObserver(Input::eInputEvent::eSelect, this);
	myPollingStation->myInputMapper.get()->RemoveObserver(Input::eInputEvent::eReleaseArrowLeft, this);
	myPollingStation->myInputMapper.get()->RemoveObserver(Input::eInputEvent::eReleaseArrowRight, this);
}

void OptionsState::Init()
{}

PopInfo OptionsState::Update(const float aDeltaTime)
{
	if (!myIsActive)
	{
		myIsActive = true;
	}
	if (myShouldChangeVolume)
	{
		myVolumeTimer += aDeltaTime;
		if (myVolumeChangeSpeed < myVolumeTimer)
		{
			myVolumeTimer = 0;
			int index = mySelectedArrow % 2;
			switch (mySelectedButton)
			{
			case eButtonType::MasterVol:
				VolumeChange(Channels::Master, index);
				break;
			case eButtonType::MusicVol:
				VolumeChange(Channels::Music, index);
				break;
			case eButtonType::SFXVol:
				VolumeChange(Channels::SFX, index);
				break;
			default:
				break;
			}
		}

	}
	if (myShouldChangeScreenRes)
	{
		Tga2D::Engine::GetInstance()->SetResolution(myScreenResolutions[myScreenResIndex], true);
		myShouldChangeScreenRes = false;
	}

	return myPopInfo;
}

void OptionsState::RecieveEvent(const Input::eInputEvent aEvent, const float aValue)
{
	if (!myIsActive)
	{
		return;
	}

	auto changeSettings = [&](int anArrowIndex)
	{
		if (aValue < 0 && myArrows[anArrowIndex + 1].GetState() != eState::Selected)
		{
			mySelectedArrow = anArrowIndex;
			myArrows[mySelectedArrow].SetState(eState::Selected);

			switch (mySelectedButton)
			{
			case eButtonType::Resolution:
				if (myScreenResIndex > 0)
				{
					myScreenResIndex--;
					myButtons[mySelectedButtonIndex].SetText(std::to_string(myScreenResolutions[myScreenResIndex].x) + "x" + std::to_string(myScreenResolutions[myScreenResIndex].y));
					myShouldChangeScreenRes = true;
				}
				break;
			default:
				break;
			}
		}
		else if (aValue > 0 && myArrows[anArrowIndex - 1].GetState() != eState::Selected)
		{
			mySelectedArrow = anArrowIndex;
			myArrows[mySelectedArrow].SetState(eState::Selected);

			switch (mySelectedButton)
			{
			case eButtonType::Resolution:
				if (myScreenResIndex < myScreenResolutions.size()-1)
				{
					myScreenResIndex++;
					myButtons[mySelectedButtonIndex].SetText(std::to_string(myScreenResolutions[myScreenResIndex].x) + "x" + std::to_string(myScreenResolutions[myScreenResIndex].y));
					myShouldChangeScreenRes = true;
				}
				break;
			default:
				break;
			}
		}
	};

	switch (aEvent)
	{
	case Input::eInputEvent::eMenuLeft:
		if (mySelectedButton == eButtonType::Resolution)
		{
			mySelectedArrow = (int)eArrowIndex::eResDown;
			changeSettings(mySelectedArrow);
		}
		else if (mySelectedButton == eButtonType::MasterVol)
		{
			myShouldChangeVolume = true;
			mySelectedArrow = (int)eArrowIndex::eMasterDown;
		}
		else if (mySelectedButton == eButtonType::MusicVol)
		{
			myShouldChangeVolume = true;
			mySelectedArrow = (int)eArrowIndex::eMusicDown;
		}
		else if (mySelectedButton == eButtonType::SFXVol)
		{
			myShouldChangeVolume = true;
			mySelectedArrow = (int)eArrowIndex::eSFXDown;
		}
		myArrows[(int)mySelectedArrow].SetState(eState::Selected);
		if(mySelectedButton!=eButtonType::SFXVol)
			myPollingStation->myAudioManager->PlayEvent(FSPRO::Event::sfx_menu_menu_hoover);
		break;

	case Input::eInputEvent::eMenuRight:
		if (mySelectedButton == eButtonType::Resolution)
		{
			mySelectedArrow = (int)eArrowIndex::eResUp;
			changeSettings(mySelectedArrow);
		}
		else if (mySelectedButton == eButtonType::MasterVol)
		{
			myShouldChangeVolume = true;
			mySelectedArrow = (int)eArrowIndex::eMasterUp;
		}
		else if (mySelectedButton == eButtonType::MusicVol)
		{
			myShouldChangeVolume = true;
			mySelectedArrow = (int)eArrowIndex::eMusicUp;
		}
		else if (mySelectedButton == eButtonType::SFXVol)
		{
			myShouldChangeVolume = true;
			mySelectedArrow = (int)eArrowIndex::eSFXUp;
		}
		myArrows[(int)mySelectedArrow].SetState(eState::Selected);
		if (mySelectedButton != eButtonType::SFXVol)
			myPollingStation->myAudioManager->PlayEvent(FSPRO::Event::sfx_menu_menu_hoover);
		break;

	case Input::eInputEvent::eMenuDown:
		myButtons[mySelectedButtonIndex].ResetColour();

		if (myButtons[mySelectedButtonIndex].GetState() == eState::Selected)
		{
			myButtons[mySelectedButtonIndex].SetState(eState::None);

			if (mySelectedButtonIndex < myButtons.size() - 1)
			{
				mySelectedButtonIndex++;
				myButtons[mySelectedButtonIndex].SetState(eState::Selected);
				
			}
			else
			{
				mySelectedButtonIndex = 0;
				myButtons[mySelectedButtonIndex].SetState(eState::Selected);
			}
			mySelectedButton = myButtons[mySelectedButtonIndex].GetType();
			myButtons[mySelectedButtonIndex].SetActiveColour();
			myPollingStation->myAudioManager->PlayEvent(FSPRO::Event::sfx_menu_menu_hoover);
			//play sound here
		}
		break;

	case Input::eInputEvent::eMenuUp:
		myButtons[mySelectedButtonIndex].ResetColour();

		if (myButtons[mySelectedButtonIndex].GetState() == eState::Selected)
		{
			myButtons[mySelectedButtonIndex].SetState(eState::None);

			if (mySelectedButtonIndex > 0)
			{
				mySelectedButtonIndex--;
				myButtons[mySelectedButtonIndex].SetState(eState::Selected);
			}
			else
			{
				mySelectedButtonIndex = (int)myButtons.size() - 1;
				myButtons[mySelectedButtonIndex].SetState(eState::Selected);
			}
			mySelectedButton = myButtons[mySelectedButtonIndex].GetType();
			myButtons[mySelectedButtonIndex].SetActiveColour();
			myPollingStation->myAudioManager->PlayEvent(FSPRO::Event::sfx_menu_menu_hoover);
			//play sound here
		}
		break;

	case Input::eInputEvent::eReleaseArrowUp:
		if (mySelectedArrow < myArrows.size())
			myArrows[mySelectedArrow].SetState(eState::None);
		myShouldChangeVolume = false;
		break;

	case Input::eInputEvent::eReleaseArrowDown:
		if (mySelectedArrow < myArrows.size())
			myArrows[mySelectedArrow].SetState(eState::None);
		myShouldChangeVolume = false;
		break;

	case Input::eInputEvent::eReleaseArrowRight:
		if (mySelectedArrow < myArrows.size())
			myArrows[mySelectedArrow].SetState(eState::None);
		myShouldChangeVolume = false;
		if (mySelectedButton == eButtonType::SFXVol)
			myPollingStation->myAudioManager->PlayEvent(FSPRO::Event::sfx_player_attack);
		break;

	case Input::eInputEvent::eReleaseArrowLeft:
		if (mySelectedArrow < myArrows.size())
			myArrows[mySelectedArrow].SetState(eState::None);
		myShouldChangeVolume = false;
		if (mySelectedButton == eButtonType::SFXVol)
			myPollingStation->myAudioManager->PlayEvent(FSPRO::Event::sfx_player_attack);
		break;

	case Input::eInputEvent::eSelect:
		InvokeButton(mySelectedButton);
		myIsActive = false;
		break;

	default:
		break;
	}
}

void OptionsState::Render()
{
	Tga2D::Engine::GetInstance()->GetGraphicsEngine().GetSpriteDrawer().Draw(mySharedData, mySpriteInstance);
	for (Button b : myButtons)
	{
		b.Render();
	}
	for (Button a : myArrows)
	{
		a.Render();
	}
}

void OptionsState::InvokeButton(eButtonType aType)
{
	switch (aType)
	{
	case eButtonType::FullScreen:
		myFullScreen = !myFullScreen;
		Tga2D::Engine::GetInstance()->SetFullScreen(myFullScreen);
		if (myButtons[mySelectedButtonIndex].GetType() == eButtonType::FullScreen)
		{
			myButtons[mySelectedButtonIndex].ToggleFullScreen();
		}
		break;
	case eButtonType::Back:
		myPopInfo.myShouldPop = true;
		myPopInfo.myNumberOfPops = 1;
		break;
	default:
		break;
	}
}

void OptionsState::VolumeChange(Channels aChannel, int aModulus)
{
	float volume = myPollingStation->myAudioManager.get()->GetVolume(aChannel) * 100;
	if (aModulus == 0)//lower volume
	{
		volume--;
		if (volume < 0)
			volume = 0;

		myPollingStation->myAudioManager.get()->SetChannelVolume(aChannel, volume / 100);
		myButtons[mySelectedButtonIndex].SetText(std::to_string((int)volume));
	}
	else if (aModulus == 1)//raise volume
	{
		volume++;
		if (volume > 100)
			volume = 100;

		myPollingStation->myAudioManager.get()->SetChannelVolume(aChannel, volume / 100);
		myButtons[mySelectedButtonIndex].SetText(std::to_string((int)volume));
	}
}