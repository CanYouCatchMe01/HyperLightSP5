#include "stdafx.h"
#include "GameState.h"
#include "PollingStation.h"
#include "Postmaster.h"
#include "StateStack.h"
#include "PauseMenuState.h"
#include "DialogueState.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameDataManager.h"
#include "Hud.h"
#include "MapState.h"
#include <iostream>
#include <tga2d/graphics/Camera.h>
#include "AudioManager.h"

GameState::GameState(StateStack& aStateStack, PollingStation* aPollingStation, const std::string& aSceneName)
	:
	State(aStateStack, aPollingStation), myHud(aPollingStation)
{
	SetPollingStation(aPollingStation);
	myPollingStation->myInputMapper.get()->AddObserver(Input::eInputEvent::eEscape, this);
	myPollingStation->myInputMapper.get()->AddObserver(Input::eInputEvent::eMap, this);
	myPollingStation->myInputMapper.get()->AddObserver(Input::eInputEvent::eGodMode, this);

	myPollingStation->myPostmaster.get()->AddObserver(this, eMessageType::ePickUpKey);

	myPollingStation->mySceneManager->LoadScene(aSceneName);
	GameData& gameData = myPollingStation->myGameDataManager.get()->GetGameData();
	gameData.myTeleporterStatus[4] = true;

	if (myPollingStation->mySceneManager->GetActiveScene()->name == "Tutorial")
	{
		myIsTutorial = true;
	}
}

GameState::~GameState()
{
	myPollingStation->myPostmaster.get()->RemoveObserver(this, eMessageType::ePickUpKey);
	myPollingStation->myInputMapper.get()->RemoveObserver(Input::eInputEvent::eEscape, this);
	myPollingStation->myInputMapper.get()->RemoveObserver(Input::eInputEvent::eMap, this);
	myPollingStation->mySceneManager->UnloadAllScenes();
}

int GameState::Update(const float aDeltaTime)
{
	myIsActive = true;

	myPollingStation->mySceneManager->Update(aDeltaTime);
	if (!myIsTutorial && !myIsDialogue)
		return myNumberOfPops;

	else if (myIsTutorial && myStartDialogueTimer > 0)
	{
		myStartDialogueTimer -= aDeltaTime;
		if (myStartDialogueTimer <= 0)
		{
			myIsActive = false;
			myStateStack.PushState(new DialogueState(myStateStack, myPollingStation));
		}
	}
	else if (myIsDialogue && myStartDialogueTimer > 0)
	{
		myStartDialogueTimer -= aDeltaTime;
		if (myStartDialogueTimer <= 0)
		{
			myIsActive = false;
			myStateStack.PushState(new DialogueState(myStateStack, myPollingStation));
		}
	}
	return myNumberOfPops;
}

void GameState::Init()
{}

void GameState::Render()
{
	myPollingStation->mySceneManager->Render();
	myHud.Render();
}

void GameState::RecieveEvent(const Input::eInputEvent aEvent, const float /*aValue*/)
{
	GameData& gameData = myPollingStation->myGameDataManager.get()->GetGameData();
	if (myIsActive)
	{
		switch (aEvent)
		{
		case Input::eInputEvent::eEscape:
			myStateStack.PushState(new PauseMenuState(myStateStack, myPollingStation));
			myIsActive = false;
			break;
		case Input::eInputEvent::eMap:
			if (myPollingStation->mySceneManager->GetActiveScene()->name == "Tutorial" && !myGodModeActive) break;

			myStateStack.PushState(new MapState(myStateStack, myPollingStation));
			myIsActive = false;
			break;
		case Input::eInputEvent::eGodMode:
			myGodModeActive = true;
			gameData.myTeleporterStatus[0] = true;
			gameData.myTeleporterStatus[1] = true;
			gameData.myTeleporterStatus[2] = true;
			gameData.myTeleporterStatus[3] = true;
			gameData.myTeleporterStatus[4] = true;
			break;
		default:
			break;
		}
	}
}

void GameState::RecieveMsg(const Message& aMsg)
{
	if (myIsActive)
	{
		switch (aMsg.myMsg)
		{
		case eMessageType::ePickUpKey:
			myIsDialogue = true;
			myStartDialogueTimer = 0.2f;
			break;
		default:
			break;
		}
	}
}
