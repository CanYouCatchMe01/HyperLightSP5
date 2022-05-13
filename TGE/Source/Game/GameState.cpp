#include "stdafx.h"
#include "GameState.h"
#include "PollingStation.h"
#include "StateStack.h"
#include "PauseMenuState.h"
#include "SceneManager.h"
#include "Hud.h"
#include <iostream>
#include <tga2d/graphics/Camera.h>

GameState::GameState(StateStack& aStateStack, PollingStation* aPollingStation) 
    :
    State(aStateStack, aPollingStation), myHud(aPollingStation)
{
    myPollingStation->myInputMapper.get()->AddObserver(Input::eInputEvent::ePause, this);
    myPopInfo.myShouldPop = false;
}

GameState::~GameState()
{
    myPollingStation->myInputMapper.get()->RemoveObserver(Input::eInputEvent::ePause, this);
}

PopInfo GameState::Update(const float aDeltaTime)
{
    myIsActive = true;

    myPollingStation->mySceneManager->Update(aDeltaTime);

    return myPopInfo;
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
        case Input::eInputEvent::ePause:
            myStateStack.PushState(new PauseMenuState(myStateStack, myPollingStation));
            myIsActive = false;
            break;
        default:
            break;
        }
    }
}
