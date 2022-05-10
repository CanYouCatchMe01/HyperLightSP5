#include "stdafx.h"
#include "OptionsState.h"
#include "Button.h"
#include "StateStack.h"

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
	mySpriteInstance.mySizeMultiplier = { 2.f,1.f };
	mySpriteInstance.myScaleSpritesWithAspectRatio = true;

	float leftArrowX = 0.38f;
	float rightArrowX = 0.62f;
	float yPosition = 0.35f;
	float yIncrement = 0.12f;
	myButtons.push_back(new Button(eButtonType::FullScreen, { 0.5f, yPosition }));
	yPosition += yIncrement;
	myButtons.push_back(new Button(eButtonType::Resolution, { 0.5f, yPosition }));
	myArrows.push_back(new Button(eButtonType::ArrowLeft, { leftArrowX,yPosition }));
	myArrows.push_back(new Button(eButtonType::ArrowRight, { rightArrowX,yPosition }));
	yPosition += yIncrement;
	myButtons.push_back(new Button(eButtonType::MasterVol, { 0.5f, yPosition }));
	myArrows.push_back(new Button(eButtonType::ArrowLeft, { leftArrowX,yPosition }));
	myArrows.push_back(new Button(eButtonType::ArrowRight, { rightArrowX,yPosition }));
	yPosition += yIncrement;
	myButtons.push_back(new Button(eButtonType::MusicVol, { 0.5f, yPosition }));
	myArrows.push_back(new Button(eButtonType::ArrowLeft, { leftArrowX,yPosition }));
	myArrows.push_back(new Button(eButtonType::ArrowRight, { rightArrowX,yPosition }));
	yPosition += yIncrement;
	myButtons.push_back(new Button(eButtonType::SFXVol, { 0.5f, yPosition }));
	myArrows.push_back(new Button(eButtonType::ArrowLeft, { leftArrowX,yPosition }));
	myArrows.push_back(new Button(eButtonType::ArrowRight, { rightArrowX,yPosition }));
	yPosition += yIncrement;
	myButtons.push_back(new Button(eButtonType::Back, { 0.5f, yPosition }));

	myButtons[0]->SetState(eState::Selected);
	mySelectedButton = myButtons[0]->GetType();
	mySelectedButtonIndex = 0;

	myPollingStation->myInputMapper.get()->AddObserver(Input::eInputEvent::eMenuUp, this);
	myPollingStation->myInputMapper.get()->AddObserver(Input::eInputEvent::eMenuDown, this);
	myPollingStation->myInputMapper.get()->AddObserver(Input::eInputEvent::eMenuLeft, this);
	myPollingStation->myInputMapper.get()->AddObserver(Input::eInputEvent::eMenuRight, this);
	myPollingStation->myInputMapper.get()->AddObserver(Input::eInputEvent::eSelect, this);
	myPollingStation->myInputMapper.get()->AddObserver(Input::eInputEvent::eReleaseUp, this);
	myPollingStation->myInputMapper.get()->AddObserver(Input::eInputEvent::eReleaseDown, this);
	myPollingStation->myInputMapper.get()->AddObserver(Input::eInputEvent::eReleaseLeft, this);
	myPollingStation->myInputMapper.get()->AddObserver(Input::eInputEvent::eReleaseRight, this);
	myPopInfo.myShouldPop = false;

	//myRes = Tga2D::Text(L"Text/Nectar.ttf", Tga2D::FontSize_18);
	//myRes.SetText("testinginging");
	//myRes.SetColor({ 1,0,0,1 });
}

OptionsState::~OptionsState()
{
	myPollingStation->myInputMapper.get()->RemoveObserver(Input::eInputEvent::eMenuUp, this);
	myPollingStation->myInputMapper.get()->RemoveObserver(Input::eInputEvent::eMenuDown, this);
	myPollingStation->myInputMapper.get()->RemoveObserver(Input::eInputEvent::eMenuLeft, this);
	myPollingStation->myInputMapper.get()->RemoveObserver(Input::eInputEvent::eMenuRight, this);
	myPollingStation->myInputMapper.get()->RemoveObserver(Input::eInputEvent::eSelect, this);
	myPollingStation->myInputMapper.get()->RemoveObserver(Input::eInputEvent::eReleaseUp, this);
	myPollingStation->myInputMapper.get()->RemoveObserver(Input::eInputEvent::eReleaseDown, this);
	myPollingStation->myInputMapper.get()->RemoveObserver(Input::eInputEvent::eReleaseLeft, this);
	myPollingStation->myInputMapper.get()->RemoveObserver(Input::eInputEvent::eReleaseRight, this);

	for (Button* b : myButtons)
	{
		delete b;
	}
	for (Button* a : myArrows)
	{
		delete a;
	}
}

void OptionsState::Init()
{}

PopInfo OptionsState::Update(const float /*aDeltaTime*/)
{
	if (!myIsActive)
	{
		myIsActive = true;
	}
	return myPopInfo;
}

void OptionsState::RecieveEvent(const Input::eInputEvent aEvent, const float /*aValue*/)
{
	if (myIsActive)
	{
		switch (aEvent)
		{
		case Input::eInputEvent::eMenuLeft:
			if (mySelectedButton == eButtonType::Resolution &&
				myArrows[(int)eArrowIndex::eResUp]->GetState() != eState::Selected)
			{
				myArrows[(int)eArrowIndex::eResDown]->SetState(eState::Selected);
				mySelectedArrow = (int)eArrowIndex::eResDown;
			}
			else if (mySelectedButton == eButtonType::MasterVol &&
				myArrows[(int)eArrowIndex::eMasterUp]->GetState() != eState::Selected)
			{
				myArrows[(int)eArrowIndex::eMasterDown]->SetState(eState::Selected);
				mySelectedArrow = (int)eArrowIndex::eMasterDown;
			}

			else if (mySelectedButton == eButtonType::MusicVol &&
				myArrows[(int)eArrowIndex::eMusicUp]->GetState() != eState::Selected)
			{
				myArrows[(int)eArrowIndex::eMusicDown]->SetState(eState::Selected);
				mySelectedArrow = (int)eArrowIndex::eMusicDown;
			}

			else if (mySelectedButton == eButtonType::SFXVol &&
				myArrows[(int)eArrowIndex::eSFXUp]->GetState() != eState::Selected)
			{
				myArrows[(int)eArrowIndex::eSFXDown]->SetState(eState::Selected);
				mySelectedArrow = (int)eArrowIndex::eSFXDown;
			}
			break;
		case Input::eInputEvent::eMenuRight:
			if (mySelectedButton == eButtonType::Resolution &&
				myArrows[(int)eArrowIndex::eResDown]->GetState() != eState::Selected)
			{
				myArrows[(int)eArrowIndex::eResUp]->SetState(eState::Selected);
				mySelectedArrow = (int)eArrowIndex::eResUp;
			}

			else if (mySelectedButton == eButtonType::MasterVol &&
				myArrows[(int)eArrowIndex::eMasterDown]->GetState() != eState::Selected)
			{
				myArrows[(int)eArrowIndex::eMasterUp]->SetState(eState::Selected);
				mySelectedArrow = (int)eArrowIndex::eMasterUp;
			}

			else if (mySelectedButton == eButtonType::MusicVol &&
				myArrows[(int)eArrowIndex::eMusicDown]->GetState() != eState::Selected)
			{
				myArrows[(int)eArrowIndex::eMusicUp]->SetState(eState::Selected);
				mySelectedArrow = (int)eArrowIndex::eMusicUp;
			}

			else if (mySelectedButton == eButtonType::SFXVol &&
				myArrows[(int)eArrowIndex::eSFXDown]->GetState() != eState::Selected)
			{
				myArrows[(int)eArrowIndex::eSFXUp]->SetState(eState::Selected);
				mySelectedArrow = (int)eArrowIndex::eSFXUp;
			}
			break;
		case Input::eInputEvent::eMenuDown:
			myButtons[mySelectedButtonIndex]->ResetColour();
			for (int i = 0; i < myButtons.size(); i++) //Cycle down
			{
				if (myButtons[i]->GetState() == eState::Selected)
				{
					myButtons[i]->SetState(eState::None);

					if (i < myButtons.size() - 1)
					{
						myButtons[i + 1]->SetState(eState::Selected);
						mySelectedButton = myButtons[i + 1]->GetType();
						mySelectedButtonIndex = i + 1;
					}
					else
					{
						myButtons[0]->SetState(eState::Selected);
						mySelectedButton = myButtons[0]->GetType();
						mySelectedButtonIndex = 0;
					}
					myButtons[mySelectedButtonIndex]->SetActiveColour();

					return;

				}
			}
			break;
		case Input::eInputEvent::eMenuUp:
			myButtons[mySelectedButtonIndex]->ResetColour();
			for (int i = 0; i < myButtons.size(); i++) //Cycle up
			{
				if (myButtons[i]->GetState() == eState::Selected)
				{
					myButtons[i]->SetState(eState::None);

					if (i > 0)
					{
						myButtons[i - 1]->SetState(eState::Selected);
						mySelectedButton = myButtons[i - 1]->GetType();
						mySelectedButtonIndex = i - 1;
					}
					else
					{
						myButtons[myButtons.size() - 1]->SetState(eState::Selected);
						mySelectedButton = myButtons[myButtons.size() - 1]->GetType();
						mySelectedButtonIndex = (int)myButtons.size() - 1;
					}
					myButtons[mySelectedButtonIndex]->SetActiveColour();

					return;
				}
			}
			break;
		case Input::eInputEvent::eReleaseUp:
			if (mySelectedArrow < myArrows.size())
			{
				myArrows[mySelectedArrow]->SetState(eState::None);
			}
			break;
		case Input::eInputEvent::eReleaseDown:
			if (mySelectedArrow < myArrows.size())
			{
				myArrows[mySelectedArrow]->SetState(eState::None);
			}
			break;
		case Input::eInputEvent::eReleaseRight:
			if (mySelectedArrow < myArrows.size())
			{
				myArrows[mySelectedArrow]->SetState(eState::None);
			}
			break;
		case Input::eInputEvent::eReleaseLeft:
			if (mySelectedArrow < myArrows.size())
			{
				myArrows[mySelectedArrow]->SetState(eState::None);
			}
			break;
		case Input::eInputEvent::eSelect:
			InvokeButton(mySelectedButton);
			myIsActive = false;
			break;
		default:
			break;
		}
	}
}

void OptionsState::Render()
{
	Tga2D::Engine::GetInstance()->GetGraphicsEngine().GetSpriteDrawer().Draw(mySharedData, mySpriteInstance);
	for (Button* b : myButtons)
	{
		b->Render();
	}
	for (Button* a : myArrows)
	{
		a->Render();
	}
}

void OptionsState::InvokeButton(eButtonType aType)
{
	switch (aType)
	{
	case eButtonType::FullScreen:
		myFullScreen = !myFullScreen;
		Tga2D::Engine::GetInstance()->SetFullScreen(myFullScreen);
		break;
	case eButtonType::Back:
		myPopInfo.myShouldPop = true;
		myPopInfo.myNumberOfPops = 1;
		break;
	default:
		break;
	}
}
