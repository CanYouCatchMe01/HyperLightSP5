#include "stdafx.h"
#include "MapState.h"
#include "StateStack.h"
#include "Button.h"
#include "GameDataManager.h"
#include "AudioManager.h"

#include "PlayerComponent.h"
#include <vector>

#include <tga2d/engine.h>
#include <tga2d/texture/TextureManager.h>
#include <tga2d/graphics/GraphicsEngine.h>
#include <tga2d/graphics/Camera.h>
#include <tga2d/drawers/SpriteDrawer.h>
#include "SceneManager.h"

MapState::MapState(StateStack& aStateStack, PollingStation* aPollingStation)
	:
	State(aStateStack, aPollingStation)
{
	SetPollingStation(aPollingStation);
	mySharedData.myTexture = Tga2D::Engine::GetInstance()->GetTextureManager().GetTexture(L"Sprites/UI/Menus/Map/ui_map_map.dds");
	mySpriteInstance.myPosition = { 0.5f,0.5f };
	mySpriteInstance.mySizeMultiplier = { 0.9f ,0.9f };

	myButtons.push_back(Button(eButtonType::MapBack, { 0.3f, 0.85f }));

	myButtons.push_back(Button(eButtonType::Teleport, { 0.36f, 0.52f })); //nere till v?nster (badlands 1)
	myButtons.push_back(Button(eButtonType::Teleport, { 0.39f, 0.3f })); //uppe till v?nster (badlands 2)
	myButtons.push_back(Button(eButtonType::Teleport, { 0.55f, 0.5f })); //nere till h?ger (jungle 1)
	myButtons.push_back(Button(eButtonType::Teleport, { 0.58f, 0.34f })); //uppe till h?ger (jungle 2)

	myButtons.push_back(Button(eButtonType::Hub, { 0.5f, 0.8f }));


	myPollingStation->myInputMapper.get()->AddObserver(Input::eInputEvent::eMap, this);
	myPollingStation->myInputMapper.get()->AddObserver(Input::eInputEvent::eEscape, this);
	myPollingStation->myInputMapper.get()->AddObserver(Input::eInputEvent::eMenuDown, this);
	myPollingStation->myInputMapper.get()->AddObserver(Input::eInputEvent::eMenuUp, this);
	myPollingStation->myInputMapper.get()->AddObserver(Input::eInputEvent::eMenuLeft, this);
	myPollingStation->myInputMapper.get()->AddObserver(Input::eInputEvent::eMenuRight, this);
	myPollingStation->myInputMapper.get()->AddObserver(Input::eInputEvent::eSelect, this);
	myPollingStation->myInputMapper.get()->AddObserver(Input::eInputEvent::eDash, this);



	myTeleportPoints[0] = "Badlands 2";
	myTeleportPoints[1] = "Badlands 3";
	myTeleportPoints[2] = "Jungle 2";
	myTeleportPoints[3] = "Jungle 3";
	myTeleportPoints[4] = "Hub";


	GameData& currentData = aPollingStation->myGameDataManager.get()->GetGameData();
	for (size_t i = 1; i < currentData.myTeleporterStatus.size(); i++)
	{
		myButtons[i].SetActiveTP(currentData.myTeleporterStatus[i-1]);
	}
	myCurrentSelection = eButtonIndex::eHub;
}

MapState::~MapState()
{
	myPollingStation->myInputMapper.get()->RemoveObserver(Input::eInputEvent::eMap, this);
	myPollingStation->myInputMapper.get()->RemoveObserver(Input::eInputEvent::eEscape, this);
	myPollingStation->myInputMapper.get()->RemoveObserver(Input::eInputEvent::eMenuDown, this);
	myPollingStation->myInputMapper.get()->RemoveObserver(Input::eInputEvent::eMenuUp, this);
	myPollingStation->myInputMapper.get()->RemoveObserver(Input::eInputEvent::eMenuLeft, this);
	myPollingStation->myInputMapper.get()->RemoveObserver(Input::eInputEvent::eMenuRight, this);
	myPollingStation->myInputMapper.get()->RemoveObserver(Input::eInputEvent::eSelect, this);
	myPollingStation->myInputMapper.get()->RemoveObserver(Input::eInputEvent::eDash, this);
}

void MapState::Init()
{
}

int MapState::Update(const float /*aDeltaTime*/)
{
	if (!myIsActive)
		myIsActive = true;

	return myNumberOfPops;
}

void MapState::Render()
{
	myStateStack.RenderUnderlyingState();

	Tga2D::Engine::GetInstance()->GetGraphicsEngine().SetCamera(Tga2D::Camera());
	Tga2D::Engine::GetInstance()->GetGraphicsEngine().GetSpriteDrawer().Draw(mySharedData, mySpriteInstance);

	for (Button b : myButtons)
	{
		b.Render();
	}
}

void MapState::InvokeButton()
{
	GameData currentGameData = myPollingStation->myGameDataManager.get()->GetGameData();
	if (myCurrentSelection == eButtonIndex::eBack)
		return;

	if (currentGameData.myTeleporterStatus[myCurrentSelection-1])
	{
		if (myCurrentSelection == eButtonIndex::eHub)
		{
			myPollingStation->mySceneManager.get()->LoadScene(myTeleportPoints[myCurrentSelection-1], "HubTeleport");
		}
		else
		{
			myPollingStation->mySceneManager.get()->LoadScene(myTeleportPoints[myCurrentSelection-1], "teleporter");
		}
		myNumberOfPops = 1;
	}
}

void MapState::RecieveEvent(const Input::eInputEvent aEvent, const float /*aValue*/)
{
	if (!myIsActive)
		return;

	switch (aEvent)
	{
	case Input::eInputEvent::eEscape:
	case Input::eInputEvent::eMap:
		myNumberOfPops = 1;
		break;
	case Input::eInputEvent::eMenuDown:

		if (myCurrentSelection == eButtonIndex::eBack)
			break;

		myButtons[myCurrentSelection].SetState(eState::None);
		if (myCurrentSelection == eButtonIndex::eHub)
		{
			myCurrentSelection = eButtonIndex::eBack;
		}
		else if (myCurrentSelection == eButtonIndex::eTPJungle_1 || myCurrentSelection == eButtonIndex::eTPBadlands_1)
		{
			myCurrentSelection = eButtonIndex::eHub;
		}
		else
		{
			myCurrentSelection--;
		}
		myPollingStation->myAudioManager->PlayEvent(FSPRO::Event::sfx_menu_menu_hoover);
		myButtons[myCurrentSelection].SetState(eState::Selected);

		break;
	case Input::eInputEvent::eMenuUp:
		if (myCurrentSelection == eButtonIndex::eTPJungle_2 || myCurrentSelection == eButtonIndex::eTPBadlands_2)
			break;

		myButtons[myCurrentSelection].SetState(eState::None);
		if (myCurrentSelection == eButtonIndex::eHub)
		{
			myCurrentSelection = eButtonIndex::eTPBadlands_1;
		}
		else if (myCurrentSelection == eButtonIndex::eBack)
		{
			myCurrentSelection = eButtonIndex::eHub;
		}
		else
		{
			myCurrentSelection++;
		}
		myPollingStation->myAudioManager->PlayEvent(FSPRO::Event::sfx_menu_menu_hoover);
		myButtons[myCurrentSelection].SetState(eState::Selected);

		break;
	case Input::eInputEvent::eMenuLeft:
		if (myCurrentSelection == eButtonIndex::eBack)
			break;

		myButtons[myCurrentSelection].SetState(eState::None);
		if (myCurrentSelection > eButtonIndex::eTPBadlands_2 && myCurrentSelection < eButtonIndex::eHub)
		{
			myCurrentSelection -= 2;
		}
		else if (myCurrentSelection == eButtonIndex::eHub || (myCurrentSelection > 0 && myCurrentSelection < eButtonIndex::eTPJungle_1))
		{
			myCurrentSelection = eButtonIndex::eBack;
		}
		myPollingStation->myAudioManager->PlayEvent(FSPRO::Event::sfx_menu_menu_hoover);
		myButtons[myCurrentSelection].SetState(eState::Selected);

		break;
	case Input::eInputEvent::eMenuRight:
		if (myCurrentSelection == eButtonIndex::eTPJungle_1 || myCurrentSelection == eButtonIndex::eTPJungle_2)
			break;

		myButtons[myCurrentSelection].SetState(eState::None);
		if (myCurrentSelection > 0 && myCurrentSelection < eButtonIndex::eTPJungle_1)
		{
			myCurrentSelection += 2;
		}
		else if (myCurrentSelection == eButtonIndex::eBack)
		{
			myCurrentSelection = eButtonIndex::eHub;
		}
		myPollingStation->myAudioManager->PlayEvent(FSPRO::Event::sfx_menu_menu_hoover);
		myButtons[myCurrentSelection].SetState(eState::Selected);

		break;
	case Input::eInputEvent::eSelect:
	case Input::eInputEvent::eDash:
		if (myCurrentSelection == eButtonIndex::eBack)
		{
			myNumberOfPops = 1;
		}
		else
			InvokeButton();
		break;
	default:
		break;
	}
}
