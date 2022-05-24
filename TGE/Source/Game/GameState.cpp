#include "stdafx.h"
#include "GameState.h"
#include "PollingStation.h"
#include "Postmaster.h"
#include "StateStack.h"
#include "PauseMenuState.h"
#include "SceneManager.h"
#include "Hud.h"
#include "MapState.h"
#include <iostream>
#include <tga2d/graphics/Camera.h>

GameState::GameState(StateStack& aStateStack, PollingStation* aPollingStation) 
    :
    State(aStateStack, aPollingStation), myHud(aPollingStation)
{
    SetPollingStation(aPollingStation);
    myPollingStation->myInputMapper.get()->AddObserver(Input::eInputEvent::eEscape, this);
    myPollingStation->myInputMapper.get()->AddObserver(Input::eInputEvent::eMap, this);
}

GameState::~GameState()
{
    myPollingStation->myInputMapper.get()->RemoveObserver(Input::eInputEvent::eEscape, this);
    myPollingStation->myInputMapper.get()->RemoveObserver(Input::eInputEvent::eMap, this);
}

int GameState::Update(const float aDeltaTime)
{
    myIsActive = true;

    myPollingStation->mySceneManager->Update(aDeltaTime);

    return myNumberOfPops;
}

void GameState::Init()
{

}

void GameState::Render()
{
    myPollingStation->mySceneManager->Render();
    myHud.Render();
}

void GameState::RecieveEvent(const Input::eInputEvent aEvent, const float /*aValue*/)
{
    if (myIsActive)
    {
        switch (aEvent)
        {
        case Input::eInputEvent::eEscape:
            myStateStack.PushState(new PauseMenuState(myStateStack, myPollingStation));
            myIsActive = false;
            break;
        case Input::eInputEvent::eMap:
            myStateStack.PushState(new MapState(myStateStack, myPollingStation));
            myIsActive = false;
        default:
            break;
        }
    }
}
